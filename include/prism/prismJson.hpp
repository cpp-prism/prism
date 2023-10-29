#ifndef PRISM_PRISMJSON_HPP
#define PRISM_PRISMJSON_HPP

#include "prism.hpp"
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 8
#include <bits/error_constants.h>
#include <tuple>
namespace std {
template <typename T>
std::tuple<const char*,errc> from_chars(const char* const _First, const char* const _Last, T& _Value)
{
    std::string str = std::string(_First,_Last);
    try {
       _Value =  std::atoi(str.c_str());
       return std::make_tuple<const char*,errc>("",std::errc());
    }
    catch (const std::exception& e)
    {
       return std::make_tuple<const char*,errc>(e.what(),std::errc());
    }
}
}
#else
#include <charconv>
#endif
#include <chrono>
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

#define PRISM_IGNORE_JSON_FIELD(Class, Field) PRISM_IGNORE_FIELD(Class, Field, json)

namespace prism
{
namespace json
{

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
template <typename T>
bool is_null_string(T str)
{
    return (str == nullptr || str[0] == '\0');
}

template <>
bool is_null_string<std::string>(std::string str)
{
    return str.empty();
}

template <class>
struct jsonValueBase;
template <class T, class = void>
struct jsonValue : public jsonValueBase<jsonValue<T>>
{
    constexpr static bool undef = true;
};

template <class T>
struct jsonObjectBase;
template <class T, class = void>
struct jsonObject : public jsonObjectBase<jsonObject<T>>
{
    constexpr static bool undef = true;
};

template <class T>
struct jsonArrayBase;
template <class T, class = void>
struct jsonArray : public jsonArrayBase<jsonArray<T>>
{
    constexpr static bool undef = true;
};

template <class T, class = void>
struct jsonType
{
    using type = jsonValue<T>;
};
template <class T>
struct jsonType<T, std::enable_if_t<prism::reflection::has_md<T>() || (!utilities::has_def<prism::json::privates::jsonValue<T>>::value && !utilities::has_def<prism::json::privates::jsonArray<T>>::value && utilities::has_def<prism::json::privates::jsonObject<T>>::value), void>>
{
    using type = jsonObject<T>;
};
template <class T>
struct jsonType<T, std::enable_if_t<!prism::reflection::has_md<T>() && !utilities::has_def<prism::json::privates::jsonValue<T>>::value && !utilities::has_def<prism::json::privates::jsonObject<T>>::value && utilities::has_def<prism::json::privates::jsonArray<T>>::value, void>>
{
    using type = jsonArray<T>;
};

//=================== ================= json array ================= ==================
template <class T>
struct jsonArray<T, std::enable_if_t<prism::utilities::is_specialization<T, std::list>::value ||
                                         prism::utilities::is_specialization<T, std::vector>::value,
                                     void>> : public jsonArrayBase<jsonArray<T>>
{
    constexpr static void append_sub_values([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        auto a = value.begin();
        while (value.size() && a != value.end())
        {
            if (a != value.begin())
            {
                stream << ',';
            }
            if (identation)
            {
                if (a != value.begin())
                    stream << std::endl;
                for (int i = 0; i < identation * (level); ++i)
                    stream << " ";
            }
            using v_t = typename T::value_type;
            jsonType<v_t>::type::append_value(stream, nullptr, std::move(*a), identation, std::move(level));

            a++;
        }
    }

    constexpr static void read_sub_value([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        using ft_ = typename T::value_type;
        model.emplace_back();
        ft_& v = model.back();
        jsonType<ft_>::type::from_jsonStr(std::move(v), std::move(str), start, end);
    }
};
//=================== ================= json object ================= ==================
template <class T>
struct jsonObject<T, std::enable_if_t<prism::reflection::has_md<T>(), void>> : public jsonObjectBase<jsonObject<T>>
{
    using jsonObjectBase<jsonObject<T>>::append_value;

    constexpr static void append_sub_kvs([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        int count = 0;
        prism::reflection::for_each_fields<prism::utilities::const_hash("json")>(value, [&](const char* ffname, auto&& value_) {
            if (count)
            {
                stream << ',';
            }
            if (identation)
            {
                if (count)
                    stream << std::endl;
                for (int i = 0; i < identation * (level); ++i)
                    stream << " ";
            }
            using v_t = std::remove_reference_t<std::remove_reference_t<decltype(value_)>>;
            jsonType<v_t>::type::append_key_value(stream, ffname, std::move(value_), identation, std::move(level));

            ++count;
        });
    }

    static void read_sub_kv([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int kstart, [[maybe_unused]] int kend, [[maybe_unused]] int vstart, [[maybe_unused]] int vend)
    {
        auto s = std::string(str.substr(kstart, kend - kstart));
        prism::reflection::field_do<prism::utilities::const_hash("json")>(model, s.c_str(), [&](auto&& value) {
            using ft_ = std::remove_reference_t<std::remove_reference_t<decltype(value)>>;
            jsonType<ft_>::type::from_jsonStr(std::move(value), std::move(str), vstart, vend);
        });
    }
};
template <class T>
struct jsonObject<T, std::enable_if_t<prism::utilities::is_specialization<T, std::map>::value ||
                                          prism::utilities::is_specialization<T, std::unordered_map>::value,
                                      void>> : public jsonObjectBase<jsonObject<T>>
{
    constexpr static void append_sub_kvs([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        int count = 0;
        for (auto& [k, v] : value)
        {
            if (count)
            {
                stream << ',';
            }
            if (identation)
            {
                if (count)
                    stream << std::endl;
                for (int i = 0; i < identation * (level); ++i)
                    stream << " ";
            }
            using v_t = typename T::value_type::second_type;
            jsonType<v_t>::type::append_key_value(stream, k.c_str(), std::move(v), identation, std::move(level));
            ++count;
        }
    }
    static void read_sub_kv([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int kstart, [[maybe_unused]] int kend, [[maybe_unused]] int vstart, [[maybe_unused]] int vend)
    {
        auto s = std::string(str.substr(kstart, kend - kstart));
        using ft_ = typename T::value_type::second_type;
        jsonType<ft_>::type::from_jsonStr(std::move(model[s]), std::move(str), vstart, vend);
    }
};
//=================== ================= json value ================= ==================

template <class T>
struct jsonValue<T, std::enable_if_t<!prism::reflection::has_md<T>() && (!utilities::has_def<jsonValue<T>>::value && utilities::has_def<jsonArray<T>>::value), void>> : public jsonValueBase<jsonValue<T>>
{
    template <class TT>
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] TT&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        jsonArray<TT>::append_value(stream, fname, std::move(value), identation, level);
    }

    template <class TT>
    static void from_jsonStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        jsonArray<TT>::from_jsonStr(std::move(model), std::move(str), start, end);
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<prism::reflection::has_md<T>() || (!utilities::has_def<jsonValue<T>>::value && utilities::has_def<jsonObject<T>>::value), void>> : public jsonValueBase<jsonValue<T>>
{
    template <class TT>
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] TT&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        jsonObject<TT>::append_value(stream, fname, std::move(value), identation, std::move(level));
    }

    template <class TT>
    static void from_jsonStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        jsonObject<TT>::from_jsonStr(std::move(model), std::move(str), start, end);
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<prism::utilities::has_def<prism::enums::enum_info<T>>::value,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        const char* str = prism::enums::enum_info<T>::tostring(value);
        if (str != nullptr)
            stream << "\"" << str << "\"";
        else
            stream << "null";
    }

    static void from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if (end - start == 3 &&
            (str[start] == 't' || str[start] == 'T') &&
            (str[start + 1] == 'r' || str[start + 1] == 'R') &&
            (str[start + 2] == 'u' || str[start + 2] == 'U') &&
            (str[start + 3] == 'e' || str[start + 3] == 'E'))
        {
            model = prism::enums::enum_info<T>::fromstring(nullptr);
        }
        else
        {
            model = prism::enums::enum_info<T>::fromstring(std::string(str.substr(start + 1, end - start - 1)).c_str());
        }
    }
};
template <class T>
struct jsonValue<T, std::enable_if_t<std::is_same_v<bool, T>,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        stream << std::boolalpha << value;
    }

    static void from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if (end - start == 3 &&
            (str[start] == 't' || str[start] == 'T') &&
            (str[start + 1] == 'r' || str[start + 1] == 'R') &&
            (str[start + 2] == 'u' || str[start + 2] == 'U') &&
            (str[start + 3] == 'e' || str[start + 3] == 'E'))
        {
            model = true;
        }
        else
        {
            model = false;
        }
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<std::is_same_v<char*, T> ||
                                         std::is_same_v<const char*, T> ||
                                         std::is_same_v<std::string, T>,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        if (!is_null_string(value))
        {
            stream << '"';
            for (const auto& ch : value)
            {
                if (ch == '"')
                {
                    stream << "\\\"";
                }
                else
                {
                    stream << ch;
                }
            }
            stream << '"';
        }
        else
        {
            stream << "null";
        }
    }

    static void from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if (str[start] == '"')
        {
            bool isEscaped = false;
            std::string unquotedStr;
            for (int i = start; i < end; ++i)
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
            model = unquotedStr;
        }
        else if (!(end - start == 3 &&
                   str[start] == 'n' &&
                   str[start + 1] == 'u' &&
                   str[start + 2] == 'l' &&
                   str[start + 3] == 'l'))
        {
            throw R"("c++ type is string, but json str not start with '"' or null")";
        }
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<std::is_same_v<T, std::chrono::system_clock::time_point>,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        stream << '"';
        std::time_t timestamp = std::chrono::system_clock::to_time_t(value);
        stream << std::put_time(std::localtime(&timestamp), "%Y-%m-%dT%H:%M:%SZ");
        stream << '"';
    }

    static void from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if (str[start] == '"')
        {
            std::string unquotedStr;
            bool isEscaped = false;

            for (int i = start; i < end; ++i)
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
            std::time_t timestamp = std::mktime(&timeInfo);
            model = std::chrono::system_clock::from_time_t(timestamp);
        }
        else
            throw R"(the string of json value does not start with ")";
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<std::is_arithmetic_v<T> &&
                                         !std::is_same_v<bool, T>,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        if constexpr(std::is_same_v<uint8_t,T>)
                stream << static_cast<int>(value);
        else
            stream << value;
    }

    template <class TT>
    static std::enable_if_t<std::is_same_v<TT, float>,
                            void>
    from_jsonStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        model = static_cast<float>(std::atof(str.substr(start, end - start).data()));
    }

    template <class TT>
    static std::enable_if_t<std::is_same_v<TT, double>,
                            void>
    from_jsonStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        model = std::atof(str.substr(start, end - start).data());
    }

    template <class TT>
    static std::enable_if_t<!std::is_same_v<TT, float> &&
                                !std::is_same_v<T, double>,
                            void>
    from_jsonStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        auto tmp = str.substr(start, end - start + 1);
        TT c;
        auto [ptr, ec] = std::from_chars(tmp.data(), tmp.data() + tmp.size(), c);

        if (ec == std::errc())
        {
            model = c;
        }
        else if (ec == std::errc::invalid_argument)
        {
            std::cout << "That isn't a number:" << std::string(str.substr(start, end - start)) << std::endl;
        }
        else if (ec == std::errc::result_out_of_range)
            std::cout << "This number out of range:" << std::string(str.substr(start, end - start)) << std::endl;
        else
            std::cout << "atoi error, a is :" << tmp << std::endl;
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<prism::utilities::is_specialization<T, std::optional>::value,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        if (value.has_value())
        {
            jsonType<typename T::value_type>::type::append_value(stream, nullptr, std::move(value.value()), identation, std::move(level));
        }
        else
        {
            stream << "null";
        }
    }

    void static from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        if (!(end - start == 3 &&
              str[start] == 'n' &&
              str[start + 1] == 'u' &&
              str[start + 2] == 'l' &&
              str[start + 3] == 'l'))
        {
            using obj_t = typename T::value_type;
            model = std::make_optional<obj_t>();
            jsonType<obj_t>::type::from_jsonStr(std::move(model.value()), std::move(str), start, end);
        }
        else
            model = std::nullopt;
    }
};

template <class T>
struct jsonValue<T, std::enable_if_t<std::is_pointer_v<T> ||
                                         prism::utilities::is_specialization<T, std::shared_ptr>::value ||
                                         prism::utilities::is_specialization<T, std::weak_ptr>::value ||
                                         prism::utilities::is_specialization<T, std::unique_ptr>::value,
                                     void>> : public jsonValueBase<jsonValue<T>>
{
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        if (!value)
        {
            stream << "null";
        }
        else
        {
            using dpt = std::remove_cv_t<std::remove_reference_t<decltype(*value)>>;
            if constexpr (prism::utilities::is_specialization<T, std::weak_ptr>::value)
            {
                // jsonValue<dpt>::append_value(stream, nullptr, std::move(*(value.lock())), identation, std::move(level));
            }
            else
                jsonType<dpt>::type::append_value(stream, nullptr, std::move(*value), identation, std::move(level));
        }
    }

    void static from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        using obj_t = std::remove_cv_t<std::remove_reference_t<decltype(*model)>>;
        if (!(end - start == 3 &&
              str[start] == 'n' &&
              str[start + 1] == 'u' &&
              str[start + 2] == 'l' &&
              str[start + 3] == 'l'))
        {
            if constexpr (prism::utilities::is_specialization<T, std::weak_ptr>::value ||
                          prism::utilities::is_specialization<T, std::shared_ptr>::value)
            {
                if constexpr (prism::utilities::is_specialization<T, std::weak_ptr>::value)
                {
                    // jsonValue<obj_t>::from_jsonStr(std::move(*(model.lock())), std::move(str), start, end);
                }
                else
                {
                    model = std::make_shared<obj_t>();
                    jsonType<obj_t>::type::from_jsonStr(std::move(*model), std::move(str), start, end);
                }
            }
            else if constexpr (prism::utilities::is_specialization<T, std::unique_ptr>::value)
            {
                model = std::make_unique<obj_t>();
                jsonType<obj_t>::type::from_jsonStr(std::move(*model), std::move(str), start, end);
            }
            else if constexpr (std::is_pointer_v<T>)
            {
                model = new obj_t();
                jsonType<obj_t>::type::from_jsonStr(std::move(*model), std::move(str), start, end);
            }
        }
        else
        {
            // model = obj_t{};
        }
    }
};
//=================== ================= json type bases ================= ==================
template <class derived>
struct jsonArrayBase : public jsonValueBase<jsonArrayBase<derived>>
{
    template <class TT>
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] TT&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        ++level;
        stream << '[';
        if (identation && value.size())
            stream << std::endl;
        derived::append_sub_values(stream, nullptr, std::move(value), identation, std::move(level));
        if (level && identation && value.size())
        {
            stream << std::endl;
            for (int i = 0; i < identation * (level - 1); ++i)
                stream << " ";
        }
        stream << ']';
        --level;
    }

    template <class TT>
    constexpr static void from_jsonStr([[maybe_unused]] TT&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        model = TT{};
        int count_quote = 0;  //"
        int count_brace = 0;  //{
        int count_braket = 0; //[
        int pre_char_idx = -1;
        int pre_valid_char_idx = -1;
        int first_valid_char_idx = -1;
        int first_valid_char_idx_l2 = -1;

        /*
        int key_idx_start = -1;
        int key_idx_end = -1;
        int colon_idx = -1;
         */
        int value_Idx_start = -1;
        int value_Idx_end = -1;
        int comma_idx = -1;

        for (int i = start; i < end + 1; ++i)
        {
            const char& c = str[i];
            // std::cout << c;

            if ((pre_char_idx != -1 && str[pre_char_idx] != '\\' || pre_char_idx == -1) && c == '"')
                ++count_quote;
            if (count_quote % 2 == 0 && c == '[')
                ++count_braket;
            if (count_quote % 2 == 0 && c == ']')
                --count_braket;
            if (count_quote % 2 == 0 && c == '{')
                ++count_brace;
            if (count_quote % 2 == 0 && c == '}')
                --count_brace;

            if (first_valid_char_idx_l2 == -1 && first_valid_char_idx != -1 && !is_whitespace(c))
            {
                first_valid_char_idx_l2 = i;
            }
            if (first_valid_char_idx == -1 && !is_whitespace(c))
            {
                first_valid_char_idx = i;
            }
            if (first_valid_char_idx != -1)
            {
                const char& first_valid_char = str[first_valid_char_idx];
                if (first_valid_char == '[')
                {
                    if (first_valid_char_idx_l2 != -1 && value_Idx_start == -1 && !is_whitespace(c))
                        value_Idx_start = i;
                    else if (value_Idx_start != -1 && value_Idx_end == -1 && !(count_quote % 2) && ((c == ',' && count_brace + count_braket == 1) || (count_braket == 0 && c == ']')))
                    {

                        value_Idx_end = pre_valid_char_idx;

                        if (value_Idx_start != value_Idx_end)
                        {
                            derived::read_sub_value(std::move(model), std::move(str), value_Idx_start, value_Idx_end + 1);
                        }

                        value_Idx_start = -1;
                        value_Idx_end = -1;
                        comma_idx = -1;
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
                                    throw R"(After the ']' of the json array is closed, there is still a non-empty string)";
                                }
                            }
                        }
                    }
                    if (i == end) //字符串检测完了
                    {
                        if (count_quote % 2)
                        {
                            throw R"("'"' is not closed, but the json string of json array is over)";
                        }
                        if (count_braket)
                        {
                            throw R"("'"' is not closed, but the json string of json array is over)";
                        }
                        if (count_brace)
                        {
                            throw R"("'"' is not closed, but the json string of json array is over)";
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
                        const char* msg = "json value string malformed: json structure does not match c++ structure";
                        throw msg;
                    }
                }
            }
            pre_char_idx = i;
            if (count_quote % 2 == 0 && !is_whitespace(c))
                pre_valid_char_idx = i;
        }
    }
};
template <class derived>
struct jsonObjectBase : public jsonValueBase<jsonObjectBase<derived>>
{
    template <class T>
    constexpr static void append_sub_kvs([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        derived::append_sub_kvs(stream, fname, std::move(value), identation, std::move(level));
    }
    template <class T>
    constexpr static void append_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        ++level;
        stream << '{';
        if (identation)
            stream << std::endl;
        append_sub_kvs(stream, fname, std::move(value), identation, std::move(level));
        if (level && identation)
        {
            stream << std::endl;
            for (int i = 0; i < identation * (level - 1); ++i)
                stream << " ";
        }
        stream << '}';
        --level;
    }
    template <class T>
    static void read_sub_kv([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int kstart, [[maybe_unused]] int kend, [[maybe_unused]] int vstart, [[maybe_unused]] int vend)
    {
        derived::read_sub_kv(std::move(model), std::move(str), kstart, kend, vstart, vend);
    }
    template <class T>
    static void from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        int count_quote = 0;  //"
        int count_brace = 0;  //{
        int count_braket = 0; //[
        int pre_char_idx = -1;
        int pre_valid_char_idx = -1;
        int first_valid_char_idx = -1;
        int first_valid_char_idx_l2 = -1;

        int key_idx_start = -1;
        int key_idx_end = -1;
        int colon_idx = -1;
        int value_Idx_start = -1;
        int value_Idx_end = -1;
        int comma_idx = -1;
        int item_count = 0;

        for (int i = start; i < end + 1; ++i)
        {
            const char& c = str[i];
            // std::cout << c;

            if ((pre_char_idx != -1 && str[pre_char_idx] != '\\' || pre_char_idx == -1) && c == '"')
                ++count_quote;
            if (count_quote % 2 == 0 && c == '[')
                ++count_braket;
            if (count_quote % 2 == 0 && c == ']')
                --count_braket;
            if (count_quote % 2 == 0 && c == '{')
                ++count_brace;
            if (count_quote % 2 == 0 && c == '}')
                --count_brace;

            if (first_valid_char_idx_l2 == -1 && first_valid_char_idx != -1 && !is_whitespace(c))
            {
                first_valid_char_idx_l2 = i;
            }
            if (first_valid_char_idx == -1 && !is_whitespace(c))
            {
                first_valid_char_idx = i;
            }
            if (first_valid_char_idx != -1)
            {
                const char& first_valid_char = str[first_valid_char_idx];
                if (first_valid_char == '{')
                {
                    if (key_idx_start == -1 && colon_idx == -1 && (count_quote % 2) == 1)
                        key_idx_start = i + 1;
                    else if (key_idx_start != -1 && key_idx_end == -1 && colon_idx == -1 && (count_quote % 2) == 0)
                        key_idx_end = i;
                    else if (key_idx_end != -1 && colon_idx == -1 && c == ':')
                        colon_idx = i;
                    else if (colon_idx != -1 && value_Idx_start == -1 && !is_whitespace(c))
                        value_Idx_start = i;
                    else if (value_Idx_start != -1 && value_Idx_end == -1 && comma_idx == -1 && !(count_quote % 2) && ((c == ',' && count_braket + count_brace == 1) || (count_brace == 0 && c == '}')))
                    {
                        value_Idx_end = pre_valid_char_idx;
                        if (c == ',')
                            comma_idx = i;
                        if (c == ',')
                            comma_idx = i;
                        if constexpr (prism::reflection::has_md<T>())
                        {
                            derived::read_sub_kv(std::move(model), std::move(str), key_idx_start, key_idx_end, value_Idx_start, value_Idx_end);
                        }
                        else if constexpr (prism::utilities::is_specialization<T, std::map>::value ||
                                           prism::utilities::is_specialization<T, std::unordered_map>::value)
                        {
                            if (item_count == 0)
                                model = T{};
                            derived::read_sub_kv(std::move(model), std::move(str), key_idx_start, key_idx_end, value_Idx_start, value_Idx_end);
                        }
                        else if constexpr (utilities::has_def<jsonObject<T>>::value)
                        {
                            if (item_count == 0)
                                model = T{};
                            derived::read_sub_kv(std::move(model), std::move(str), key_idx_start, key_idx_end, value_Idx_start, value_Idx_end);
                        }
                        else
                        {
                            const char* msg = "The c++ type of the json object does not match, please check the c++ type";
                            throw msg;
                        }
                        ++item_count;

                        key_idx_start = -1;
                        key_idx_end = -1;
                        colon_idx = -1;
                        value_Idx_start = -1;
                        value_Idx_end = -1;
                        comma_idx = -1;
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
                                    throw R"(After the '}' of the json object is closed, there is still a non-empty string)";
                                }
                            }
                        }
                    }
                    if (i == end)
                    {
                        if (count_quote % 2)
                        {
                            throw R"("'"' is not closed, but the json string of json object is over)";
                        }
                        if (count_braket)
                        {
                            throw R"("']' is not closed, but the json string of json object is over)";
                        }
                        if (count_brace)
                        {
                            throw R"("'}' is not closed, but the json string of json object is over)";
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
                        const char* msg = "json value string malformed: json structure does not match c++ structure";
                        throw msg;
                    }
                }
            }
            pre_char_idx = i;
            if (count_quote % 2 == 0 && !is_whitespace(c))
                pre_valid_char_idx = i;
        }
    }
};
template <class derived>
struct jsonValueBase
{
    template <class T>
    constexpr static void append_key_value([[maybe_unused]] std::ostringstream& stream, [[maybe_unused]] const char* fname, [[maybe_unused]] T&& value, [[maybe_unused]] int identation, [[maybe_unused]] int&& level)
    {
        if (fname)
            stream << '\"' << fname << "\":";
        derived::append_value(stream, fname, std::move(value), identation, std::move(level));
    }

    template <class T>
    constexpr static void from_jsonStr([[maybe_unused]] T&& model, [[maybe_unused]] std::string_view&& str, [[maybe_unused]] int start, [[maybe_unused]] int end)
    {
        derived::from_jsonStr(std::move(model), std::move(str), start, end);
    }
};

} // namespace privates

} // namespace json

} // namespace prism

namespace prism
{
namespace json
{
template <class T>
static inline std::string toJsonString(T&& model, int identation = 0)
{
    std::string str;
    std::ostringstream sstream(str);
    int level = 0;
    privates::jsonType<T>::type::append_value(sstream, nullptr, std::move(model), identation, std::move(level));
    return sstream.str();
}
template <class T>
static inline std::string toJsonString(T& model, int identation = 0)
{
    return toJsonString(std::move(model), identation);
}

template <class T>
static inline std::shared_ptr<T> fromJsonString(const std::string&& str)
{
    std::shared_ptr<T> model = std::make_shared<T>();
    privates::jsonType<T>::type::from_jsonStr(std::move(*model), std::move(str), 0, static_cast<int>(str.length() - 1));

    return model;
}
template <class T>
static inline std::shared_ptr<T> fromJsonString(const std::string& str)
{
    return fromJsonString<T>(std::move(str));
}
} // namespace json

} // namespace prism

#endif // PRISM_PRISMJSON_HPP
