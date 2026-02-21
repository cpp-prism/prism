#ifndef PRISM_PRISMYAML_HPP
#define PRISM_PRISMYAML_HPP

#include "prism.hpp"
#include "prismJson.hpp"
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 8
#include <bits/error_constants.h>
#include <tuple>
namespace std
{
template <typename T>
std::tuple<const char*, errc> from_chars(const char* const _First, const char* const _Last, T& _Value)
{
    std::string str = std::string(_First, _Last);
    try
    {
        _Value = std::atoi(str.c_str());
        return std::make_tuple<const char*, errc>("", std::errc());
    }
    catch (const std::exception& e)
    {
        return std::make_tuple<const char*, errc>(e.what(), std::errc());
    }
}
} // namespace std
#else
#include <charconv>
#endif
#include <chrono>
#include <ctime>
#include <exception>
#include <iomanip>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <vector>

#if defined(_MSC_VER)
// timegm is POSIX; MSVC provides _mkgmtime
inline std::time_t timegm(std::tm* t) { return _mkgmtime(t); }
#endif

#define PRISM_IGNORE_YAML_FIELD(Class, Field) PRISM_IGNORE_FIELD(Class, Field, yaml)

namespace prism
{
namespace yaml
{

namespace attributes
{
struct Attr_yaml_ignore{
    using value_type= bool;
};
struct Attr_yaml_alias{
    using value_type= const char*;
};
// 格式属性：控制单个字段使用 block 还是 flow 格式
// 默认为 false，表示跟随父级模式
// 为 true 时，翻转格式（block -> flow, flow -> block）
struct Attr_yaml_format{
    using value_type= bool;
};

} //namespace attributes

namespace privates
{
constexpr inline static const char* WHITESPACE = " \r\n\t\0";
constexpr static inline bool is_whitespace(const char& c)
{
    int i = 0;
    while (WHITESPACE[i] != '\0')
    {
        if (WHITESPACE[i] == c)
            return true;
        ++i;
    }
    return false;
}

template <class T>
static inline bool is_null_string(T str)
{
    return (str == nullptr || str[0] == '\0');
}

template <>
inline bool is_null_string<std::string>([[maybe_unused]] std::string str)
{
    return false;
}

constexpr int YAML_INDENT = 2;

// 格式模式枚举
enum class FormatMode {
    Block,   // 块格式
    Flow     // 流格式
};

// 获取字段的格式模式（考虑 Attr_yaml_format 属性）
template <class T>
static inline FormatMode getFieldFormatMode(const std::string& field_name, FormatMode parent_mode)
{
    std::optional<bool> flip = prism::attributes::getFieldAttr<T, ::prism::yaml::attributes::Attr_yaml_format>(field_name.c_str());
    if (flip.has_value() && flip.value()) {
        // 翻转格式
        return (parent_mode == FormatMode::Block) ? FormatMode::Flow : FormatMode::Block;
    }
    return parent_mode;
}

// 前向声明
template <class T>
struct yamlValueBase;
template <class T, class = void>
struct yamlValue : public yamlValueBase<yamlValue<T>>
{
    constexpr static bool undef = true;
};

template <class T>
struct yamlObjectBase;
template <class T, class = void>
struct yamlObject : public yamlObjectBase<yamlObject<T>>
{
    constexpr static bool undef = true;
};

template <class T>
struct yamlArrayBase;
template <class T, class = void>
struct yamlArray : public yamlArrayBase<yamlArray<T>>
{
    constexpr static bool undef = true;
};

template <class T, class = void>
struct yamlType
{
    using type = yamlValue<T>;
};
template <class T>
struct yamlType<T, std::enable_if_t<prism::reflection::has_md<T>() || (!utilities::has_def<yamlValue<T>>::value && !utilities::has_def<yamlArray<T>>::value && utilities::has_def<yamlObject<T>>::value), void>>
{
    using type = yamlObject<T>;
};
template <class T>
struct yamlType<T, std::enable_if_t<!prism::reflection::has_md<T>() && !utilities::has_def<yamlValue<T>>::value && !utilities::has_def<yamlObject<T>>::value && utilities::has_def<yamlArray<T>>::value, void>>
{
    using type = yamlArray<T>;
};

// ================== YAML Array for list and vector =================
template <class T>
struct yamlArray<T, std::enable_if_t<prism::utilities::is_specialization<T, std::list>::value ||
                                         prism::utilities::is_specialization<T, std::vector>::value,
                                     void>> : public yamlArrayBase<yamlArray<T>>
{
    static void append_sub_values([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (fname) {
            stream << fname << ":";
        }
        // 当作为结构体字段值序列化时（fname==nullptr, level>0），
        // 在元素前换行并将缩进加深一级，使解析器能通过缩进检测子内容。
        bool nested_field_mode = (fname == nullptr && level > 0 && identation);
        if (value.empty()) {
            if (identation) {
                if (nested_field_mode) {
                    // 空容器写 [] 让解析器能识别并清空字段（否则字段保留默认值）
                    stream << "[]";
                } else {
                    stream << std::endl;
                }
            }
            // flow mode: outer append_value already wraps with [], output nothing here
            return;
        }
        if (fname && identation) {
            stream << std::endl;
        }

        int element_level = nested_field_mode ? (level + 1) : level;
        if (nested_field_mode) {
            stream << std::endl;
        }

        auto a = value.begin();
        int count = 0;
        while (a != value.end())
        {
            if (identation) {
                for (int i = 0; i < YAML_INDENT * element_level; ++i)
                    stream << " ";
                stream << "- ";
            } else {
                if (count > 0) {
                    stream << ", ";
                }
            }
            using v_t = typename T::value_type;
            privates::yamlType<v_t>::type::append_value(stream, nullptr, std::move(*a), identation, element_level + 1);
            if (identation) {
                stream << std::endl;
            }
            a++;
            count++;
        }
    }

    static void read_sub_value([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        using ft_ = typename T::value_type;
        model.emplace_back();
        ft_& v = model.back();
        privates::yamlType<ft_>::type::from_yamlStr(std::move(v), std::move(str), start, end);
    }
};

// ================== YAML Array for deque =================
template <class T>
struct yamlArray<T, std::enable_if_t<prism::utilities::is_specialization<T, std::deque>::value,
                                     void>> : public yamlArrayBase<yamlArray<T>>
{
    static void append_sub_values([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (fname) {
            stream << fname << ":";
        }
        bool nested_field_mode = (fname == nullptr && level > 0 && identation);
        if (value.empty()) {
            if (identation) {
                if (nested_field_mode) {
                    stream << "[]";
                } else {
                    stream << std::endl;
                }
            }
            return;
        }
        if (fname && identation) {
            stream << std::endl;
        }

        int element_level = nested_field_mode ? (level + 1) : level;
        if (nested_field_mode) {
            stream << std::endl;
        }

        auto a = value.begin();
        int count = 0;
        while (a != value.end())
        {
            if (identation) {
                for (int i = 0; i < YAML_INDENT * element_level; ++i)
                    stream << " ";
                stream << "- ";
            } else {
                if (count > 0) {
                    stream << ", ";
                }
            }
            using v_t = typename T::value_type;
            privates::yamlType<v_t>::type::append_value(stream, nullptr, std::move(*a), identation, element_level + 1);
            if (identation) {
                stream << std::endl;
            }
            a++;
            count++;
        }
    }

    static void read_sub_value([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        using ft_ = typename T::value_type;
        ft_ v;
        privates::yamlType<ft_>::type::from_yamlStr(std::move(v), std::move(str), start, end);
        model.push_back(std::move(v));
    }
};

// ================== YAML Array for set =================
template <class T>
struct yamlArray<T, std::enable_if_t<prism::utilities::is_specialization<T, std::set>::value ||
                                         prism::utilities::is_specialization<T, std::unordered_set>::value,
                                     void>> : public yamlArrayBase<yamlArray<T>>
{
    static void append_sub_values([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (fname) {
            stream << fname << ":";
        }
        bool nested_field_mode = (fname == nullptr && level > 0 && identation);
        if (value.empty()) {
            if (identation) {
                if (nested_field_mode) {
                    stream << "[]";
                } else {
                    stream << std::endl;
                }
            }
            return;
        }
        if (fname && identation) {
            stream << std::endl;
        }

        int element_level = nested_field_mode ? (level + 1) : level;
        if (nested_field_mode) {
            stream << std::endl;
        }

        auto a = value.begin();
        int count = 0;
        while (a != value.end())
        {
            if (identation) {
                for (int i = 0; i < YAML_INDENT * element_level; ++i)
                    stream << " ";
                stream << "- ";
            } else {
                if (count > 0) {
                    stream << ", ";
                }
            }
            using v_t = typename T::value_type;
            using iter_ref_t = decltype(*a);
            if constexpr (std::is_const_v<std::remove_reference_t<iter_ref_t>>) {
                v_t tmp = *a;
                privates::yamlType<v_t>::type::append_value(stream, nullptr, std::move(tmp), identation, element_level + 1);
            } else {
                privates::yamlType<v_t>::type::append_value(stream, nullptr, std::move(*a), identation, element_level + 1);
            }
            if (identation) {
                stream << std::endl;
            }
            a++;
            count++;
        }
    }

    static void read_sub_value([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        using ft_ = typename T::value_type;
        ft_ value;
        privates::yamlType<ft_>::type::from_yamlStr(std::move(value), std::move(str), start, end);
        model.insert(std::move(value));
    }
};

// ================== YAML Object =================
template <class T>
struct yamlObject<T, std::enable_if_t<prism::reflection::has_md<T>(), void>> : public yamlObjectBase<yamlObject<T>>
{
    using yamlObjectBase<yamlObject<T>>::append_value;
    using yamlObjectBase<yamlObject<T>>::alias_map_;
    using yamlObjectBase<yamlObject<T>>::append_sub_kvs;
    using yamlObjectBase<yamlObject<T>>::append_sub_kvs_with_key;

    static void append_sub_kvs([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        bool has_fields = false;
        prism::reflection::for_each_fields<prism::utilities::const_hash("yaml")>(value, [&](const char* ffname, auto&& value_) {
            has_fields = true;
            std::optional<bool> isignore = prism::attributes::getFieldAttr<T,::prism::yaml::attributes::Attr_yaml_ignore>(ffname);
            if(!isignore.has_value()) {
                isignore = prism::attributes::getFieldAttr<T,::prism::json::attributes::Attr_json_ignore>(ffname);
            }
            if(isignore.has_value() && isignore.value()) {
                return;
            }
            std::optional<const char*> attr = prism::attributes::getFieldAttr<T,::prism::yaml::attributes::Attr_yaml_alias>(ffname);
            if(!attr.has_value()) {
                attr = prism::attributes::getFieldAttr<T,::prism::json::attributes::Attr_json_alias>(ffname);
            }
            std::string alias = ffname;
            if(attr.has_value()) {
                alias = attr.value();
            }
            if (identation)
            {
                for (int i = 0; i < YAML_INDENT * level; ++i)
                    stream << " ";
            }
            using v_t = std::remove_reference_t<std::remove_reference_t<decltype(value_)>>;
            // 块格式：先输出键名，然后换行
            if (identation) {
                stream << alias << ": ";
            }
            privates::yamlType<v_t>::type::append_value(stream, nullptr, std::move(value_), identation, level);
            if (identation) {
                stream << std::endl;
            }
        });
        // If no fields were processed, output empty object
        if (!has_fields) {
            stream << "{}";
        }
    }

    static void append_sub_kvs_with_key([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level, [[maybe_unused]] int& count)
    {
        prism::reflection::for_each_fields<prism::utilities::const_hash("yaml")>(value, [&](const char* ffname, auto&& value_) {
            std::optional<bool> isignore = prism::attributes::getFieldAttr<T,::prism::yaml::attributes::Attr_yaml_ignore>(ffname);
            if(!isignore.has_value()) {
                isignore = prism::attributes::getFieldAttr<T,::prism::json::attributes::Attr_json_ignore>(ffname);
            }
            if(isignore.has_value() && isignore.value()) {
                return;
            }
            std::optional<const char*> attr = prism::attributes::getFieldAttr<T,::prism::yaml::attributes::Attr_yaml_alias>(ffname);
            if(!attr.has_value()) {
                attr = prism::attributes::getFieldAttr<T,::prism::json::attributes::Attr_json_alias>(ffname);
            }
            std::string alias = ffname;
            if(attr.has_value()) {
                alias = attr.value();
            }
            if (count > 0) {
                stream << ", ";
            }
            using v_t = std::remove_reference_t<std::remove_reference_t<decltype(value_)>>;
            // 流格式：使用 append_value_with_key 添加键名
            privates::yamlType<v_t>::type::append_value_with_key(stream, alias.c_str(), std::move(value_), identation, level);
            ++count;
        });
    }

    static void read_sub_kv([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int kstart, [[maybe_unused]] int kend, [[maybe_unused]] int vstart, [[maybe_unused]] int vend)
    {
        auto s = std::string(str.substr(kstart, kend - kstart + 1));

        if(!alias_map_.size())
        {
            prism::reflection::for_each_fields<prism::utilities::const_hash("yaml")>(model, [&](const char* fname ,[[maybe_unused]]auto&& value) {
                std::optional<bool> isignore = prism::attributes::getFieldAttr<T,::prism::yaml::attributes::Attr_yaml_ignore>(fname);
                if(!isignore.has_value()) {
                    isignore = prism::attributes::getFieldAttr<T,::prism::json::attributes::Attr_json_ignore>(fname);
                }
                if(isignore.has_value() && isignore)
                    return;
                std::optional<const char*> attr = prism::attributes::getFieldAttr<T,::prism::yaml::attributes::Attr_yaml_alias>(fname);
                if(!attr.has_value()) {
                    attr = prism::attributes::getFieldAttr<T,::prism::json::attributes::Attr_json_alias>(fname);
                }
                std::string alias = fname;
                if(attr.has_value())
                    alias = attr.value();
                alias_map_[alias] = fname;
            });
        }
        std::string alias = alias_map_[s];

        // 检查是否是空值
        bool is_empty_value = true;
        for (int i = vstart; i <= vend; ++i)
        {
            if (!is_whitespace(str[i]))
            {
                is_empty_value = false;
                break;
            }
        }


        prism::reflection::field_do<prism::utilities::const_hash("yaml")>(model, alias.c_str(), [&](auto&& value) {
            using ft_ = std::remove_reference_t<std::remove_reference_t<decltype(value)>>;
            if (is_empty_value)
            {
                // 空值处理：根据类型解析为空值
                if constexpr (prism::utilities::is_specialization<ft_, std::optional>::value)
                {
                    value = std::nullopt;
                }
                else if constexpr (std::is_pointer_v<ft_>)
                {
                    value = nullptr;
                }
                else if constexpr (utilities::has_def<yamlArray<ft_>>::value ||
                                   prism::utilities::is_specialization<ft_, std::list>::value ||
                                   prism::utilities::is_specialization<ft_, std::vector>::value)
                {
                    // 容器类型：保持默认空状态
                }
                else if constexpr (prism::utilities::is_specialization<ft_, std::shared_ptr>::value ||
                                   prism::utilities::is_specialization<ft_, std::unique_ptr>::value ||
                                   prism::utilities::is_specialization<ft_, std::weak_ptr>::value)
                {
                    // 智能指针：设置为 nullptr
                    value = nullptr;
                }
                else
                {
                    // 其他类型：使用默认构造
                    value = ft_{};
                }
            }
            else
            {
                privates::yamlType<ft_>::type::from_yamlStr(std::move(value), std::move(str), vstart, vend);
            }
        });
    }
};

template <class T>
struct yamlObject<T, std::enable_if_t<prism::utilities::is_specialization<T, std::map>::value ||
                                          prism::utilities::is_specialization<T, std::unordered_map>::value,
                                      void>> : public yamlObjectBase<yamlObject<T>>
{
    using yamlObjectBase<yamlObject<T>>::append_value;
    using yamlObjectBase<yamlObject<T>>::append_sub_kvs;
    using yamlObjectBase<yamlObject<T>>::append_sub_kvs_with_key;
    using yamlObjectBase<yamlObject<T>>::read_sub_kv;
    using yamlObjectBase<yamlObject<T>>::from_yamlStr;

    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (identation > 0) {
            // 块格式
            ++level;
            if (fname) {
                stream << fname << ":" << std::endl;
            } else if (!value.empty()) {
                // 作为结构体字段值调用（父级已写 "key: "），写换行使条目另起一行
                stream << std::endl;
            }
            if (value.size())
            {
                append_sub_kvs(stream, fname, std::forward<T>(value), identation, level);
            }
            --level;
        } else {
            // 流格式
            stream << '{';
            int count = 0;
            append_sub_kvs_with_key(stream, fname, std::forward<T>(value), 0, level, count);
            stream << '}';
        }
    }

    static void append_sub_kvs([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        for (auto& [k, v] : value)
        {
            if (identation)
            {
                for (int i = 0; i < YAML_INDENT * level; ++i)
                    stream << " ";
            }
            using v_t = typename T::value_type::second_type;
            if (identation) {
                stream << k << ": ";
            }
            privates::yamlType<v_t>::type::append_value(stream, nullptr, std::move(v), identation, level);
            if (identation) {
                stream << std::endl;
            }
        }
    }

    static void append_sub_kvs_with_key([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level, [[maybe_unused]] int& count)
    {
        for (auto& [k, v] : value)
        {
            if (count > 0) {
                stream << ", ";
            }
            using v_t = typename T::value_type::second_type;
            privates::yamlType<v_t>::type::append_value_with_key(stream, k.c_str(), std::move(v), identation, level);
            ++count;
        }
    }
    static void read_sub_kv([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int kstart, [[maybe_unused]] int kend, [[maybe_unused]] int vstart, [[maybe_unused]] int vend)
    {
        auto s = std::string(str.substr(kstart, kend - kstart + 1));
        using ft_ = typename T::value_type::second_type;
        privates::yamlType<ft_>::type::from_yamlStr(std::move(model[s]), std::move(str), vstart, vend);
    }
};

// ================== YAML Value =================

// 通用Value类型：处理对象类型
template <class T>
struct yamlValue<T, std::enable_if_t<prism::reflection::has_md<T>() || (!utilities::has_def<yamlValue<T>>::value && utilities::has_def<yamlObject<T>>::value), void>> : public yamlValueBase<yamlValue<T>>
{
    template <class TT>
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] TT&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        yamlObject<TT>::append_value(stream, fname, std::forward<TT>(value), identation, level);
    }

    template <class TT>
    static void from_yamlStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        yamlObject<TT>::from_yamlStr(std::forward<TT>(model), std::forward<std::string_view>(str), start, end);
    }
};

// 枚举类型
template <class T>
struct yamlValue<T, std::enable_if_t<prism::utilities::has_def<prism::enums::enum_info<T>>::value,
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        const char* str = prism::enums::enum_info<T>::tostring(value);
        if (str != nullptr)
            stream << str;
        else
            stream << "null";
    }

    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string enumStr;
        bool quoted = false;

        if (start < end && str[start] == '"')
        {
            quoted = true;
            for (int i = start + 1; i < end; ++i)
            {
                if (str[i] == '"' && (i == start + 1 || str[i-1] != '\\'))
                    break;
                enumStr += str[i];
            }
        }
        else
        {
            for (int i = start; i <= end; ++i)
            {
                if (!is_whitespace(str[i]))
                    enumStr += str[i];
            }
        }

        model = prism::enums::enum_info<T>::fromstring(enumStr.c_str());
    }
};

// 布尔类型
template <class T>
struct yamlValue<T, std::enable_if_t<std::is_same_v<bool, T>,
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        stream << (value ? "true" : "false");
    }

    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string boolStr;
        for (int i = start; i <= end; ++i)
        {
            if (!is_whitespace(str[i]))
                boolStr += str[i];
        }

        std::string lowerStr;
        for (char c : boolStr) lowerStr += static_cast<char>(std::tolower(c));

        model = (lowerStr == "true" || lowerStr == "y" || lowerStr == "yes" || lowerStr == "on");
    }
};

// 字符串类型
template <class T>
struct yamlValue<T, std::enable_if_t<!std::is_arithmetic_v<T> &&
                                         (std::is_same_v<char*, T> ||
                                          std::is_same_v<const char*, T> ||
                                          std::is_same_v<std::string, T>),
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (!is_null_string(value))
        {
            std::string strVal = value;
            bool needsQuotes = false;

            if (strVal.empty())
                needsQuotes = true;
            else if (strVal.find_first_of(" \t\n\r:#{}[],&*?|>'\"\\%") != std::string::npos)
                needsQuotes = true;
            else if (strVal[0] == '-')
                needsQuotes = true;
            else
            {
                std::string lowerStr = strVal;
                for (char& c : lowerStr) c = std::tolower(c);
                if (lowerStr == "true" || lowerStr == "false" || lowerStr == "null" ||
                    lowerStr == "yes" || lowerStr == "no" || lowerStr == "on" || lowerStr == "off")
                    needsQuotes = true;
            }

            if (needsQuotes)
            {
                stream << '"';
                for (const auto& ch : strVal)
                {
                    if (ch == '"') stream << "\\\"";
                    else if (ch == '\\') stream << "\\\\";
                    else if (ch == '\n') stream << "\\n";
                    else if (ch == '\r') stream << "\\r";
                    else if (ch == '\t') stream << "\\t";
                    else stream << ch;
                }
                stream << '"';
            }
            else
            {
                stream << strVal;
            }
        }
        else
        {
            stream << "null";
        }
    }

    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string unquotedStr;
        bool isEscaped = false;
        bool wasQuoted = false;

        if (start < end && str[start] == '"')
        {
            wasQuoted = true;
            for (int i = start + 1; i < end; ++i)
            {
                auto& c = str[i];
                if (isEscaped)
                {
                    unquotedStr += c;
                    isEscaped = false;
                }
                else if (c == '\\')
                {
                    isEscaped = true;
                }
                else if (c == '"')
                {
                    break;
                }
                else
                {
                    unquotedStr += c;
                }
            }
        }
        else
        {
            for (int i = start; i <= end; ++i)
            {
                if (!is_whitespace(str[i]))
                    unquotedStr += str[i];
            }
        }
        model = unquotedStr;
    }
};

// 数值类型（除了bool）
template <class T>
struct yamlValue<T, std::enable_if_t<std::is_arithmetic_v<T> &&
                                         !std::is_same_v<bool, T>,
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if constexpr (std::is_same_v<uint8_t, T>)
            stream << static_cast<int>(value);
        else if constexpr (std::is_same_v<float, T> || std::is_same_v<double, T>)
        {
            std::stringstream ss;
            ss << std::setprecision(17) << value;
            stream << ss.str();
        }
        else if constexpr (std::is_same_v<char, T> || std::is_same_v<unsigned char, T> ||
                          std::is_same_v<short, T> || std::is_same_v<unsigned short, T>)
            stream << static_cast<int>(value);
        else
            stream << value;
    }

    static void from_yamlStr([[maybe_unused]] float&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string numStr;
        for (int i = start; i <= end; ++i)
        {
            if (!is_whitespace(str[i]))
                numStr += str[i];
        }
        model = static_cast<float>(std::atof(numStr.c_str()));
    }

    static void from_yamlStr([[maybe_unused]] double&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string numStr;
        for (int i = start; i <= end; ++i)
        {
            if (!is_whitespace(str[i]))
                numStr += str[i];
        }
        model = std::atof(numStr.c_str());
    }

    static void from_yamlStr([[maybe_unused]] long double&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string numStr;
        for (int i = start; i <= end; ++i)
        {
            if (!is_whitespace(str[i]))
                numStr += str[i];
        }
        model = std::strtold(numStr.c_str(), nullptr);
    }

    template <class TT>
    static std::enable_if_t<!std::is_same_v<TT, float> &&
                                !std::is_same_v<TT, double> &&
                                !std::is_same_v<TT, long double>,
                            void>
    from_yamlStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        std::string numStr;
        bool wasQuoted = false;

        if (start < end && str[start] == '"')
        {
            wasQuoted = true;
            for (int i = start + 1; i <= end; ++i)
            {
                if (str[i] == '"')
                    break;
                if (!is_whitespace(str[i]))
                    numStr += str[i];
            }
        }
        else
        {
            for (int i = start; i <= end; ++i)
            {
                if (!is_whitespace(str[i]))
                    numStr += str[i];
            }
        }

        auto tmp = numStr;
        TT c;
        auto [ptr, ec] = std::from_chars(tmp.data(), tmp.data() + tmp.size(), c);

        if (ec == std::errc())
        {
            model = c;
        }
    }
};

// optional类型
template <class T>
struct yamlValue<T, std::enable_if_t<prism::utilities::is_specialization<T, std::optional>::value,
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (value.has_value())
        {
            using vt = typename T::value_type;
            if constexpr (std::is_same_v<vt, std::string>)
            {
                std::string strVal = value.value();
                stream << '"';
                for (const auto& ch : strVal)
                {
                    if (ch == '"') stream << "\\\"";
                    else if (ch == '\\') stream << "\\\\";
                    else if (ch == '\n') stream << "\\n";
                    else if (ch == '\r') stream << "\\r";
                    else if (ch == '\t') stream << "\\t";
                    else stream << ch;
                }
                stream << '"';
            }
            else
            {
                privates::yamlType<vt>::type::append_value(stream, nullptr, std::move(value.value()), identation, level);
            }
        }
        else
        {
            stream << "null";
        }
    }

    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        // YAML null 变体：~, null, Null, NULL（忽略末尾空白）
        bool isNull = false;
        if (end >= start) {
            if (str[start] == '~') {
                bool all_ws = true;
                for (int i = start + 1; i <= end; ++i)
                    if (!is_whitespace(str[i])) { all_ws = false; break; }
                if (all_ws) isNull = true;
            } else if (end - start >= 3) {
                isNull = (std::tolower(static_cast<unsigned char>(str[start])) == 'n' &&
                          std::tolower(static_cast<unsigned char>(str[start + 1])) == 'u' &&
                          std::tolower(static_cast<unsigned char>(str[start + 2])) == 'l' &&
                          std::tolower(static_cast<unsigned char>(str[start + 3])) == 'l');
            }
        }

        if (!isNull)
        {
            using obj_t = typename T::value_type;
            model = std::make_optional<obj_t>();
            privates::yamlType<obj_t>::type::from_yamlStr(std::move(model.value()), std::move(str), start, end);
        }
        else
            model = std::nullopt;
    }
};

// 指针和智能指针类型
template <class T>
struct yamlValue<T, std::enable_if_t<std::is_pointer_v<T> ||
                                         prism::utilities::is_specialization<T, std::shared_ptr>::value ||
                                         prism::utilities::is_specialization<T, std::weak_ptr>::value ||
                                         prism::utilities::is_specialization<T, std::unique_ptr>::value,
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (!value)
        {
            stream << "null";
        }
        else
        {
            using dpt = std::remove_cv_t<std::remove_reference_t<decltype(*value)>>;
            privates::yamlType<dpt>::type::append_value(stream, nullptr, std::move(*value), identation, level);
        }
    }

    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        // YAML null 变体：~, null, Null, NULL（忽略末尾空白）
        bool isNull = false;
        if (end >= start) {
            if (str[start] == '~') {
                bool all_ws = true;
                for (int i = start + 1; i <= end; ++i)
                    if (!is_whitespace(str[i])) { all_ws = false; break; }
                if (all_ws) isNull = true;
            } else if (end - start >= 3) {
                isNull = (std::tolower(static_cast<unsigned char>(str[start])) == 'n' &&
                          std::tolower(static_cast<unsigned char>(str[start + 1])) == 'u' &&
                          std::tolower(static_cast<unsigned char>(str[start + 2])) == 'l' &&
                          std::tolower(static_cast<unsigned char>(str[start + 3])) == 'l');
            }
        }

        if (!isNull)
        {
            using obj_t = std::remove_cv_t<std::remove_reference_t<decltype(*model)>>;
            if constexpr (prism::utilities::is_specialization<T, std::shared_ptr>::value)
            {
                model = std::make_shared<obj_t>();
                privates::yamlType<obj_t>::type::from_yamlStr(std::move(*model), std::move(str), start, end);
            }
            else if constexpr (prism::utilities::is_specialization<T, std::unique_ptr>::value)
            {
                model = std::make_unique<obj_t>();
                privates::yamlType<obj_t>::type::from_yamlStr(std::move(*model), std::move(str), start, end);
            }
            else if constexpr (std::is_pointer_v<T>)
            {
                model = new obj_t();
                privates::yamlType<obj_t>::type::from_yamlStr(std::move(*model), std::move(str), start, end);
            }
        }
        else
        {
            // null: 明确设置模型为空值
            if constexpr (prism::utilities::is_specialization<T, std::shared_ptr>::value)
                model = nullptr;
            else if constexpr (prism::utilities::is_specialization<T, std::unique_ptr>::value)
                model = nullptr;
            else if constexpr (std::is_pointer_v<T>)
                model = nullptr;
        }
    }
};

// time_point 类型
template <class T>
struct yamlValue<T, std::enable_if_t<std::is_same_v<T, std::chrono::system_clock::time_point>,
                                     void>> : public yamlValueBase<yamlValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        stream << '"';
        std::time_t timestamp = std::chrono::system_clock::to_time_t(value);
        stream << std::put_time(std::gmtime(&timestamp), "%Y-%m-%dT%H:%M:%SZ");
        stream << '"';
    }

    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if (str[start] == '"')
        {
            std::string unquotedStr;
            bool isEscaped = false;

            for (int i = start + 1; i < end; ++i)
            {
                auto& c = str[i];
                if (isEscaped)
                {
                    unquotedStr += c;
                    isEscaped = false;
                }
                else if (c == '\\')
                {
                    isEscaped = true;
                }
                else if (c != '"')
                {
                    unquotedStr += c;
                }
            }
            std::tm timeInfo = {};
            std::istringstream ss(unquotedStr);
            ss >> std::get_time(&timeInfo, "%Y-%m-%dT%H:%M:%SZ");
            // 使用 timegm/_mkgmtime 将 UTC 时间转换为 time_t（与序列化时使用 gmtime 保持一致）
#if defined(_WIN32) && !defined(__CYGWIN__)
            std::time_t timestamp = _mkgmtime(&timeInfo);
#else
            std::time_t timestamp = timegm(&timeInfo);
#endif
            model = std::chrono::system_clock::from_time_t(timestamp);
        }
        else
            throw R"(the string of yaml value does not start with ")";
    }
};

// ================== YAML Type Bases =================
template <class derived>
struct yamlArrayBase : public yamlValueBase<yamlArrayBase<derived>>
{
    template <class TT>
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] TT&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        // 默认使用块格式数组
        if (identation) {
            derived::append_sub_values(stream, fname, std::move(value), identation, level);
        } else {
            // 流格式数组
            stream << "[";
            derived::append_sub_values(stream, nullptr, std::move(value), identation, level);
            stream << "]";
        }
    }

    template <class TT>
    static void from_yamlStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if constexpr (std::is_copy_constructible<TT>::value)
        {
            model = TT{};
        }

        // 跳过开头的空白字符
        while (start <= end && is_whitespace(str[start]))
            ++start;

        if (start > end)
        {
            return;
        }

        // 检测是否是 null 值
        if (end - start >= 3 &&
            str[start] == 'n' &&
            str[start + 1] == 'u' &&
            str[start + 2] == 'l' &&
            (end - start == 3 || (end - start >= 4 && str[start + 3] == 'l')))
        {
            // null 值返回空 vector
            return;
        }

        // 检测是否是空行（只有空白字符）
        bool is_only_whitespace = true;
        for (int i = start; i <= end; ++i)
        {
            if (!is_whitespace(str[i]))
            {
                is_only_whitespace = false;
                break;
            }
        }
        if (is_only_whitespace)
        {
            // 空行返回空 vector
            return;
        }

        // 检测是否是块格式数组（以 `-` 开头）
        bool is_block_format = (str[start] == '-');


        if (is_block_format)
        {
            // 确定序列项 '-' 的列号，以区分外层与内嵌序列的分隔符
            int seq_col = 0;
            {
                int back = start;
                while (back > 0 && str[back - 1] != '\n')
                    --back;
                seq_col = start - back;
            }

            // 解析块格式数组
            int idx = start;
            while (idx <= end)
            {
                // 跳过空白字符
                while (idx <= end && is_whitespace(str[idx]))
                    ++idx;

                if (idx > end || str[idx] != '-')
                    break;

                // 验证此 '-' 在正确列（与第一个 '-' 列号相同）
                {
                    int back = idx;
                    while (back > 0 && str[back - 1] != '\n')
                        --back;
                    if (idx - back != seq_col)
                        break;
                }

                // 找到 `-` 后，跳过 `-` 和后面的空白字符（含换行，支持 "- \n  content" 格式）
                ++idx;
                while (idx <= end && is_whitespace(str[idx]))
                    ++idx;

                // 记录值的起始位置
                int value_start = idx;

                // 找到值的结束位置：下一个在 seq_col 列的 '-'（同级序列的下一项）
                int value_end = end;
                while (idx <= end)
                {
                    if (str[idx] == '\n')
                    {
                        // 检查下一非空行是否是同级 '-'
                        int scan = idx + 1;
                        while (scan <= end && str[scan] == '\n')
                            ++scan;
                        if (scan > end)
                        {
                            value_end = end;
                            idx = end + 1;
                            break;
                        }
                        // 计算下一行的缩进
                        int next_col = 0;
                        while (scan + next_col <= end && (str[scan + next_col] == ' ' || str[scan + next_col] == '\t'))
                            ++next_col;
                        if (scan + next_col <= end && str[scan + next_col] == '-' && next_col == seq_col)
                        {
                            // 找到同级下一项
                            value_end = idx;
                            idx = scan;
                            break;
                        }
                    }
                    ++idx;
                }

                // 提取并解析值
                if (value_start <= value_end)
                {
                    // 跳过值开头的空白
                    while (value_start <= value_end && is_whitespace(str[value_start]))
                        ++value_start;

                    // 跳过值结尾的空白
                    while (value_end >= value_start && is_whitespace(str[value_end]))
                        --value_end;

                    if (value_start <= value_end)
                    {
                        derived::read_sub_value(std::forward<TT>(model), std::forward<std::string_view>(str), value_start, value_end);
                    }
                }
            }
        }
        else
        {
            // 解析流格式数组（以 `[` 开头）
            int count_quote = 0;
            int count_brace = 0;
            int count_braket = 0;
            int pre_char_idx = -1;
            int pre_valid_char_idx = -1;
            int first_valid_char_idx = -1;

            int value_Idx_start = -1;
            int value_Idx_end = -1;

            for (int i = start; i < end + 1; ++i)
            {
                const char& c = str[i];

                if (((pre_char_idx != -1 && str[pre_char_idx] != '\\') || pre_char_idx == -1) && c == '"')
                    ++count_quote;
                if (count_quote % 2 == 0 && c == '[')
                    ++count_braket;
                if (count_quote % 2 == 0 && c == ']')
                    --count_braket;
                if (count_quote % 2 == 0 && c == '{')
                    ++count_brace;
                if (count_quote % 2 == 0 && c == '}')
                    --count_brace;

                if (first_valid_char_idx == -1 && !is_whitespace(c))
                {
                    first_valid_char_idx = i;
                }
                if (first_valid_char_idx != -1)
                {
                    const char& first_valid_char = str[first_valid_char_idx];
                    if (first_valid_char == '[')
                    {
                        if (value_Idx_start == -1 && !is_whitespace(c))
                        {
                            value_Idx_start = i;
                            if (c == '[' && count_braket == 1)
                                value_Idx_start = -1;  // 最外层 '[' 不是值，重置
                        }
                        else if (value_Idx_start != -1 && value_Idx_end == -1 && !(count_quote % 2) && ((c == ',' && count_brace + count_braket == 1) || (count_braket == 0 && c == ']')))
                        {
                            value_Idx_end = pre_valid_char_idx;

                            if (value_Idx_start != -1 && value_Idx_end != -1 && value_Idx_start <= value_Idx_end)
                            {
                                derived::read_sub_value(std::forward<TT>(model), std::forward<std::string_view>(str), value_Idx_start, value_Idx_end);
                            }

                            value_Idx_start = -1;
                            value_Idx_end = -1;
                        }

                        if (count_braket == 0)
                        {
                            if (count_quote % 2)
                            {
                                throw R"("the '"' is not closed, but ']' is closed)";
                            }
                            if (i != end)
                            {
                                for (int j = i + 1; j < end; ++j)
                                {
                                    if (!is_whitespace(str[j]))
                                    {
                                        throw R"(After the ']' of the yaml array is closed, there is still a non-empty string)";
                                    }
                                }
                            }
                        }
                        if (i == end)
                        {
                            if (count_quote % 2)
                            {
                                throw R"("'"' is not closed, but the yaml string of yaml array is over)";
                            }
                            if (count_braket)
                            {
                                throw R"("'"' is not closed, but the yaml string of yaml array is over)";
                            }
                            if (count_brace)
                            {
                                throw R"("'"' is not closed, but the yaml string of yaml array is over)";
                            }
                        }
                    }
                    else
                    {
                        if (!(end - start == 3 &&
                              str[start] == 'n' &&
                              str[start + 1] == 'u' &&
                              str[start + 2] == 'l' &&
                              str[start + 3] == 'l'))
                        {
                            const char* msg = "yaml value string malformed: yaml structure does not match c++ structure";
                            throw msg;
                        }
                    }
                }
                pre_char_idx = i;
                if (count_quote % 2 == 0 && !is_whitespace(c))
                    pre_valid_char_idx = i;
            }
        }
    }
};

template <class derived>
struct yamlObjectBase : public yamlValueBase<yamlObjectBase<derived>>
{
    static inline std::map<std::string,std::string> alias_map_;

    template <class T>
    static void append_sub_kvs([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        derived::append_sub_kvs(stream, fname, std::forward<T>(value), identation, level);
    }
    template <class T>
    static void append_sub_kvs_with_key([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level, [[maybe_unused]] int& count)
    {
        derived::append_sub_kvs_with_key(stream, fname, std::forward<T>(value), identation, level, count);
    }
    template <class T>
    static void append_value_with_key([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (fname) {
            std::string key = fname;
            bool needsQuotes = false;
            for (char c : key) {
                if (!std::isalnum(c) && c != '_' && c != '-') {
                    needsQuotes = true;
                    break;
                }
            }
            if (needsQuotes) {
                stream << '\"' << key << "\": ";
            } else {
                stream << key << ": ";
            }
        }
        derived::append_value(stream, fname, std::forward<T>(value), identation, level);
    }
    template <class T>
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (identation > 0) {
            // 块格式
            ++level;
            if (!fname && level > 1) {
                // 作为嵌套字段值调用（父级已写 "key: "），先换行使子字段另起一行
                stream << std::endl;
            }
            append_sub_kvs(stream, fname, std::forward<T>(value), identation, level);
            --level;
        } else {
            // 流格式
            stream << '{';
            int count = 0;
            append_sub_kvs_with_key(stream, fname, std::forward<T>(value), 0, level, count);
            stream << '}';
        }
    }
    template <class T>
    static void read_sub_kv([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int kstart, [[maybe_unused]] int kend, [[maybe_unused]] int vstart, [[maybe_unused]] int vend)
    {
        derived::read_sub_kv(std::forward<T>(model), std::forward<std::string_view>(str), kstart, kend, vstart, vend);
    }
    template <class T>
    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        // 跳过开头的空白字符
        int actual_start = start;
        while (actual_start <= end && is_whitespace(str[actual_start]))
            ++actual_start;

        if (actual_start > end)
            return;

        // 检测是否是块格式对象（不以 '{' 或 '[' 开头）
        bool is_block_format = (str[actual_start] != '{' && str[actual_start] != '[');

        if (is_block_format)
        {
            // 解析块格式对象（key: value 对）
            // 初始化 model
            if constexpr (std::is_copy_constructible<T>::value)
            {
                model = T{};
            }

            int idx = actual_start;
            while (idx <= end)
            {
                // 跳过空白字符
                while (idx <= end && is_whitespace(str[idx]))
                    ++idx;

                if (idx > end)
                    break;

                // 检测是否是新层级的开始（缩进后的 key），对于顶层块格式，直接找 key
                // 块格式的 key 是行首非空字符

                // 找到 key 的起始位置
                int key_start = idx;

                // 找到 key 的结束位置（':' 之前）
                int key_end = -1;
                int colon_pos = -1;
                while (idx <= end)
                {
                    if (str[idx] == ':')
                    {
                        key_end = idx - 1;
                        colon_pos = idx;
                        break;
                    }
                    if (str[idx] == '\n')
                    {
                        // 没有找到冒号就换行了，这可能是无效格式
                        break;
                    }
                    ++idx;
                }

                if (key_end < key_start || colon_pos == -1)
                    break;

                // 跳过 key 后的空白和冒号（但不包括换行符）
                idx = colon_pos + 1;
                // 先跳过空格和制表符，但保留换行符
                while (idx <= end && (str[idx] == ' ' || str[idx] == '\t'))
                    ++idx;

                // 找到 value 的起始和结束位置
                int value_start = idx;
                int value_end = -1;

                // 检查是否是空值（值只有空白字符，包括换行符）
                // 如果 idx 超过了 end 或者 idx 指向换行符，则值是空的
                bool is_empty_value_flag = false;
                if (idx > end || str[idx] == '\n')
                {
                    value_start = 1;
                    value_end = 0;
                    is_empty_value_flag = true;

                    // 计算当前 key 所在行的缩进（key_indent）
                    int key_line_pos = key_start - 1;
                    while (key_line_pos >= start && str[key_line_pos] != '\n')
                        --key_line_pos;
                    ++key_line_pos;
                    int key_indent = key_start - key_line_pos;

                    // 跳过换行符
                    while (idx <= end && str[idx] == '\n')
                        ++idx;

                    // 检查下一行的缩进是否大于 key_indent（即存在缩进子内容）
                    int next_line_start = idx;
                    int next_line_indent = 0;
                    while (idx <= end && (str[idx] == ' ' || str[idx] == '\t'))
                    {
                        ++next_line_indent;
                        ++idx;
                    }

                    if (idx <= end && str[idx] != '\n' && next_line_indent > key_indent)
                    {
                        // 发现缩进子内容，收集到缩进回到 key_indent 级别为止
                        // 保持 is_empty_value_flag=true 以跳过"find next key"循环
                        // 且避免 value_end 被覆盖
                        value_start = next_line_start;
                        idx = next_line_start + next_line_indent;
                        while (idx <= end)
                        {
                            if (str[idx] == '\n')
                            {
                                int scan = idx + 1;
                                int line_indent = 0;
                                while (scan <= end && (str[scan] == ' ' || str[scan] == '\t'))
                                {
                                    ++line_indent;
                                    ++scan;
                                }
                                if (scan > end)
                                {
                                    value_end = end;
                                    idx = end + 1;
                                    break;
                                }
                                if (str[scan] != '\n' && line_indent <= key_indent)
                                {
                                    value_end = idx;
                                    idx = scan;
                                    break;
                                }
                            }
                            ++idx;
                        }
                        if (value_end == -1)
                            value_end = end;
                    }
                    // else: 真正的空值，idx 已指向下一行首个非空白字符
                }

                // 找到下一个 key 的位置（即下一个不带缩进的 key:）
                int next_key_pos = -1;
                // 如果不是空值，才查找下一个 key
                if (!is_empty_value_flag) {
                while (idx <= end)
                {
                    if (str[idx] == '\n')
                    {
                        // 检查下一行是否是新的 key（不是缩进）
                        int next_idx = idx + 1;
                        while (next_idx <= end && is_whitespace(str[next_idx]))
                            ++next_idx;

                        if (next_idx <= end)
                        {
                            // 检查是否是新的 key（非缩进，包含冒号）
                            int scan_pos = next_idx;
                            bool has_colon = false;
                            while (scan_pos <= end && str[scan_pos] != '\n')
                            {
                                if (str[scan_pos] == ':')
                                {
                                    has_colon = true;
                                    break;
                                }
                                ++scan_pos;
                            }

                            if (has_colon)
                            {
                                // 找到下一个 key
                                next_key_pos = next_idx;
                                break;
                            }
                        }
                    }
                    ++idx;
                }
                } // end of if (!is_empty_value_flag)

                if (next_key_pos == -1 && !is_empty_value_flag)
                {
                    value_end = end;
                }
                else if (!is_empty_value_flag)
                {
                    value_end = next_key_pos - 1;
                    // 回退 value_end 到非空白字符
                    while (value_end >= value_start && is_whitespace(str[value_end]))
                        --value_end;
                }

                // 提取 key 字符串（使用临时变量，避免修改 key_start）
                std::string key_str;
                {
                    int ks = key_start;
                    int ke = key_end;
                    while (ks <= ke)
                    {
                        if (!is_whitespace(str[ks]))
                            key_str += str[ks];
                        ++ks;
                    }
                }

                // 检查是否是空值（空行或只有空白字符）
                bool is_empty_value = true;
                for (int i = value_start; i <= value_end; ++i)
                {
                    if (!is_whitespace(str[i]))
                    {
                        is_empty_value = false;
                        break;
                    }
                }

                // 调试输出

                // 递归解析 value
                if (value_start <= value_end)
                {
                    if constexpr (prism::utilities::is_specialization<T, std::map>::value ||
                                   prism::utilities::is_specialization<T, std::unordered_map>::value)
                    {
                        // 对于 map 类型，使用 read_sub_kv 来解析
                        derived::read_sub_kv(std::forward<T>(model), std::forward<std::string_view>(str), key_start, key_end, value_start, value_end);
                    }
                    else if constexpr (prism::reflection::has_md<T>())
                    {
                        // 对于有反射信息的类型，使用字段名
                        derived::read_sub_kv(std::forward<T>(model), std::forward<std::string_view>(str), key_start, key_end, value_start, value_end);
                    }
                }
                // 否则（value_start > value_end），说明是空行，跳过不解析

                if (next_key_pos == -1)
                {
                    // 如果是空值，继续循环（idx 已经被更新到下一个字段）
                    if (is_empty_value_flag)
                    {
                        // idx 已经被更新，继续循环
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    idx = next_key_pos;
                }
            }
        }
        else
        {
            // 解析流格式对象（以 '{' 开头）
            int count_quote = 0;
            int count_brace = 0;
            int count_braket = 0;
            int pre_char_idx = -1;
            int pre_valid_char_idx = -1;
            int first_valid_char_idx = -1;

            int key_idx_start = -1;
            int key_idx_end = -1;
            int colon_idx = -1;
            int value_Idx_start = -1;
            int value_Idx_end = -1;
            int item_count = 0;
            bool key_is_quoted = false;

            for (int i = start; i < end + 1; ++i)
            {
                const char& c = str[i];

                if (((pre_char_idx != -1 && str[pre_char_idx] != '\\') || pre_char_idx == -1) && c == '"')
                    ++count_quote;
                if (count_quote % 2 == 0 && c == '[')
                    ++count_braket;
                if (count_quote % 2 == 0 && c == ']')
                    --count_braket;
                if (count_quote % 2 == 0 && c == '{')
                    ++count_brace;
                if (count_quote % 2 == 0 && c == '}')
                    --count_brace;

                if (first_valid_char_idx == -1 && !is_whitespace(c))
                {
                    first_valid_char_idx = i;
                }
                if (first_valid_char_idx != -1)
                {
                    const char& first_valid_char = str[first_valid_char_idx];
                    if (first_valid_char == '{')
                    {
                        if (key_idx_start == -1 && colon_idx == -1 && count_brace == 1 && c == '"')
                        {
                            key_is_quoted = true;
                            key_idx_start = i + 1;
                        }
                        else if (key_idx_start == -1 && colon_idx == -1 && count_brace == 1 && !is_whitespace(c) && c != '{' && c != '}' && c != ',')
                        {
                            key_is_quoted = false;
                            key_idx_start = i;
                        }
                        else if (key_idx_start != -1 && key_idx_end == -1 && colon_idx == -1 && key_is_quoted && c == '"')
                            key_idx_end = i - 1;  // key_idx_end 指向 key 的最后一个字符，不包括引号
                        else if (key_idx_start != -1 && key_idx_end == -1 && colon_idx == -1 && !key_is_quoted && c == ':' && count_brace == 1)
                        {
                            key_idx_end = pre_valid_char_idx;
                            colon_idx = i;
                        }
                        else if (key_idx_end != -1 && colon_idx == -1 && c == ':')
                            colon_idx = i;  // 引号 key 的冒号
                        else if (colon_idx != -1 && value_Idx_start == -1 && !is_whitespace(c))
                            value_Idx_start = i;
                        else if (value_Idx_start != -1 && value_Idx_end == -1 && !(count_quote % 2) && ((c == ',' && count_braket + count_brace == 1) || (c == '\n' && count_brace == 1) || (count_brace == 0 && c == '}')))
                        {
                            value_Idx_end = pre_valid_char_idx;

                            if constexpr (prism::reflection::has_md<T>())
                            {
                                derived::read_sub_kv(std::forward<T>(model), std::forward<std::string_view>(str), key_idx_start, key_idx_end, value_Idx_start, value_Idx_end);
                            }
                            else if constexpr (prism::utilities::is_specialization<T, std::map>::value ||
                                               prism::utilities::is_specialization<T, std::unordered_map>::value)
                            {
                                if (item_count == 0)
                                {
                                    if constexpr (std::is_copy_constructible<T>::value)
                                        model = T{};
                                }
                                derived::read_sub_kv(std::forward<T>(model), std::forward<std::string_view>(str), key_idx_start, key_idx_end, value_Idx_start, value_Idx_end);
                            }
                            else if constexpr (utilities::has_def<yamlObject<T>>::value)
                            {
                                if (item_count == 0)
                                {
                                    if constexpr (std::is_copy_constructible<T>::value)
                                        model = T{};
                                }
                                derived::read_sub_kv(std::forward<T>(model), std::forward<std::string_view>(str), key_idx_start, key_idx_end, value_Idx_start, value_Idx_end);
                            }
                            else
                            {
                                const char* msg = "The c++ type of the yaml object does not match, please check the c++ type";
                                throw msg;
                            }
                            ++item_count;

                            key_is_quoted = false;
                            key_idx_start = -1;
                            key_idx_end = -1;
                            colon_idx = -1;
                            value_Idx_start = -1;
                            value_Idx_end = -1;
                        }

                        if (count_brace == 0)
                        {
                            if (count_quote % 2)
                            {
                                throw R"("the '"' is not closed, but '}' is closed)";
                            }
                            if (i != end)
                            {
                                for (int j = i + 1; j < end; ++j)
                                {
                                    if (!is_whitespace(str[j]))
                                    {
                                        throw R"(After the '}' of the yaml object is closed, there is still a non-empty string)";
                                    }
                                }
                            }
                        }
                        if (i == end)
                        {
                            if (count_quote % 2)
                            {
                                throw R"("'"' is not closed, but the yaml string of yaml object is over)";
                            }
                            if (count_braket)
                            {
                                throw R"("']' is not closed, but the yaml string of yaml object is over)";
                            }
                            if (count_brace)
                            {
                                throw R"("'}' is not closed, but the yaml string of yaml object is over)";
                            }
                        }
                    }
                    else
                    {
                        if (!(end - start == 3 &&
                              str[start] == 'n' &&
                              str[start + 1] == 'u' &&
                              str[start + 2] == 'l' &&
                              str[start + 3] == 'l'))
                        {
                            const char* msg = "yaml value string malformed: yaml structure does not match c++ structure";
                            throw msg;
                        }
                    }
                }
                pre_char_idx = i;
                if (count_quote % 2 == 0 && !is_whitespace(c))
                    pre_valid_char_idx = i;
            }
        }
    }
};

template <class derived>
struct yamlValueBase
{
    template <class T>
    static void append_key_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (fname) {
            std::string key = fname;
            bool needsQuotes = false;
            for (char c : key) {
                if (!std::isalnum(c) && c != '_' && c != '-') {
                    needsQuotes = true;
                    break;
                }
            }
            if (needsQuotes) {
                stream << '\"' << key << "\": ";
            } else {
                stream << key << ": ";
            }
        }
        derived::append_value(stream, fname, std::forward<T>(value), identation, level);
    }

    template <class T>
    static void append_value_with_key([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int level)
    {
        if (fname) {
            std::string key = fname;
            bool needsQuotes = false;
            for (char c : key) {
                if (!std::isalnum(c) && c != '_' && c != '-') {
                    needsQuotes = true;
                    break;
                }
            }
            if (needsQuotes) {
                stream << '\"' << key << "\": ";
            } else {
                stream << key << ": ";
            }
        }
        derived::append_value(stream, fname, std::forward<T>(value), identation, level);
    }

    template <class T>
    static void from_yamlStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        derived::from_yamlStr(std::forward<T>(model), std::forward<std::string_view>(str), start, end);
    }
};

} // namespace privates

} // namespace yaml

} // namespace prism

namespace prism
{
namespace yaml
{
// all_block 模式：所有结构使用块格式
template <class T>
static inline std::string toYamlStringBlock(T&& model, int identation = 2)
{
    std::string str;
    std::ostringstream sstream(str);
    int level = 0;
    privates::yamlType<std::remove_reference_t<T>>::type::append_value(sstream, nullptr, std::forward<T>(model), identation, level);
    return sstream.str();
}
template <class T>
static inline std::string toYamlStringBlock(T& model, int identation = 2)
{
    return toYamlStringBlock(std::move(model), identation);
}

// all_flow 模式：所有结构使用流格式
template <class T>
static inline std::string toYamlStringFlow(T&& model, int /*identation*/ = 2)
{
    std::string str;
    std::ostringstream sstream(str);
    int level = 0;
    privates::yamlType<std::remove_reference_t<T>>::type::append_value(sstream, nullptr, std::forward<T>(model), 0, level);
    return sstream.str();
}
template <class T>
static inline std::string toYamlStringFlow(T& model, int identation = 2)
{
    return toYamlStringFlow(std::move(model), identation);
}

// 默认使用 all_block 模式
template <class T>
static inline std::string toYamlString(T&& model, int identation = 2)
{
    return toYamlStringBlock(std::forward<T>(model), identation);
}
template <class T>
static inline std::string toYamlString(T& model, int identation = 2)
{
    return toYamlStringBlock(std::move(model), identation);
}

template <class T>
static inline std::shared_ptr<T> fromYamlString(const std::string&& str)
{
    std::shared_ptr<T> model = std::make_shared<T>();
    privates::yamlType<std::remove_reference_t<T>>::type::from_yamlStr(std::move(*model), std::forward<const std::string>(str), 0, static_cast<int>(str.length() - 1));
    return model;
}
template <class T>
static inline std::shared_ptr<T> fromYamlString(const std::string& str)
{
    return fromYamlString<T>(std::move(str));
}
} // namespace yaml

} // namespace prism

#endif // PRISM_PRISMYAML_HPP
