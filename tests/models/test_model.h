#ifndef TEST_MODEL_H
#define TEST_MODEL_H

#include "../../include/prism/prismJson.hpp"

enum language
{
    unknow,
    english,
    SimplifiedChinese,
    TraditionalChinese,
};

PRISM_ENUM(language, {{unknow, "未知"},
                      {english, "英语"},
                      {SimplifiedChinese, "简体中文"},
                      {TraditionalChinese, "瀪体中文"}})

std::time_t initTimestamp(int year, int month, int day, int hour = 0, int min = 0, int second = 0)
{
    std::tm timeInfo = {};
    timeInfo.tm_year = year - 1900; // 年份从 1900 年开始计数
    timeInfo.tm_mon = month - 1;    // 月份从 0 开始计数
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = min;
    timeInfo.tm_sec = second;

    std::time_t timestamp = std::mktime(&timeInfo);
    return timestamp;
}
std::chrono::system_clock::time_point initDatetime(int year, int month, int day, int hour = 0, int min = 0, int second = 0)
{
    std::time_t timestamp = initTimestamp(year, month, day, hour, min, second);
    return std::chrono::system_clock::from_time_t(timestamp);
}

struct tst_sub_struct
{
    int my_int = 0;
    bool my_bool = false;
    float my_float = 0;
    double my_double = 0;
    int64_t my_longlong = 0;
    std::string my_string;
    std::optional<std::string> my_opt_str;
    std::optional<int> my_opt_int;
    int* my_ptr_int = nullptr;
    std::optional<bool*> my_opt_ptr;
    std::vector<tst_sub_struct> my_vec_sp;
};
PRISM_FIELDS(tst_sub_struct, my_int, my_bool, my_float, my_double, my_string, my_longlong, my_opt_str, my_opt_int, my_ptr_int, my_vec_sp, my_opt_ptr)

struct tst_struct
{
    int my_int = 0;
    bool my_bool = false;
    float my_float = 0;
    double my_double = 0;
    int64_t my_longlong = 0;
    std::string my_string = R"(测试有"号和有 的字符串)";
    std::optional<std::string> my_opt_str;
    std::optional<int> my_opt_int;
    int* my_ptr_int;
    std::shared_ptr<int> my_sptr_int = std::make_shared<int>(2);
    std::optional<bool*> my_opt_ptr;
    tst_sub_struct my_struct;
    std::map<std::string, tst_struct> my_map;
    std::map<std::string, std::shared_ptr<tst_sub_struct>> my_map2{{"key1", std::make_shared<tst_sub_struct>()}, {"key2", {}}};
    std::unordered_map<std::string, tst_sub_struct> my_unordermap;
    std::vector<tst_sub_struct> my_vec_sp;
    std::list<tst_struct> my_list_sp;
    std::list<int> my_list_int{111, 2222};
    std::list<std::string> my_list_std_string{"111", "2222"};
    std::shared_ptr<tst_struct> my_shared_self;
    std::shared_ptr<tst_sub_struct> my_shared_sub;
    std::chrono::system_clock::time_point my_datetime;
    std::time_t my_timestamp{};
    language lang = language::SimplifiedChinese;
};

/*
 * With the macro mark, the two fields my_timestamp and my_int do not participate in json serialization,
 * unit test, json strings will be equal, but the field comparison will not be equal
 */
// PRISM_IGNORE_JSON_FIELD(tst_struct,my_timestamp)
// PRISM_IGNORE_JSON_FIELD(tst_struct,my_int)

/**
 *  json alias  attribute test
 */
PRISM_FIELD_ATTRIBUTE(&tst_struct::my_int,
                      prism::attributes::Attr_json_alias,
                      "MY_INT1")

PRISM_FIELDS(tst_struct, my_int, my_bool, my_float, my_double, my_longlong, my_string, my_opt_str, my_opt_int, my_ptr_int, my_sptr_int, my_opt_ptr, my_map, my_map2, my_unordermap, my_vec_sp, my_list_sp, my_list_int, my_list_std_string, my_shared_self, my_shared_sub, my_datetime, my_timestamp, lang)

struct Base1
{
    int b1_int = 1;
    float b1_float = 1.0f;
};
PRISM_FIELDS(Base1, b1_int, b1_float)
struct Base2
{

    int b2_int = 2;
    float b2_float = 1.0f;
};
PRISM_FIELDS(Base2, b2_int, b2_float)
struct Derived : public Base1, Base2
{
    int d_int = 3;
    float d_float = 1.0f;
};
PRISM_CLASS_BASE_TYPES(Derived, Base1, Base2)
PRISM_FIELDS(Derived, d_int, d_float)

template <class T>
constexpr void append2ostream(std::ostream& stream, T& value)
{
    using t_ = std::decay_t<decltype(value)>;
    if constexpr (std::is_same_v<t_, int> || std::is_same_v<t_, float> || std::is_same_v<t_, double> || std::is_same_v<t_, int64_t> || std::is_same_v<t_, std::string>)
        stream << value << std::endl;
    else if constexpr (std::is_same_v<t_, bool>)
        stream << (value ? "true" : "false") << std::endl;
    else if constexpr (prism::utilities::is_specialization<t_, std::optional>::value)
    {
        if (value.has_value())
            append2ostream(stream, value.value());
        else
            stream << "null" << std::endl;
    }
    else if constexpr (std::is_pointer_v<t_> ||
                       prism::utilities::is_specialization<t_, std::shared_ptr>::value ||
                       prism::utilities::is_specialization<t_, std::unique_ptr>::value)
    {
        if (value == nullptr)
            stream << "nullptr" << std::endl;
        else
            append2ostream(stream, *value);
    }
    else if constexpr (prism::reflection::has_md<t_>())
    {
        stream << "object type addreass: 0x" << reinterpret_cast<size_t>(reinterpret_cast<void*>(&value)) << std::endl;
    }
    else if constexpr (prism::utilities::is_specialization<t_, std::vector>::value ||
                       prism::utilities::is_specialization<t_, std::list>::value)
    {
        for (auto& v : value)
        {
            stream << " value:";
            append2ostream(stream, v);
        }
    }
    else if constexpr (prism::utilities::is_specialization<t_, std::map>::value ||
                       prism::utilities::is_specialization<t_, std::unordered_map>::value)
    {
        for (auto [k, v] : value)
        {
            if constexpr (std::is_same_v<std::remove_cv_t<decltype(k)>, std::string>)
            {
                stream << "key:" << k << " value:";
                append2ostream(stream, v);
            }
            else
                throw "std::pair unknow key type";
        }
    }
    else if constexpr (std::is_same_v<t_, std::chrono::system_clock::time_point>)
    {
        std::time_t time_t_now = std::chrono::system_clock::to_time_t(value);
        // 使用 std::put_time 格式化输出
        std::tm timeinfo = *std::localtime(&time_t_now);
        char buffer[80]{};
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        // 打印格式化后的时间
        stream << buffer << std::endl;
    }
    else if constexpr (prism::utilities::has_def<prism::enums::enum_info<T>>::value)
    {
        stream << prism::enums::enum_info<t_>::tostring(value) << std::endl;
    }
    else if constexpr (std::is_same_v<T, long>)
    {
        stream << value << std::endl;
    }
    else
    {
        std::cout << prism::utilities::typeName<t_>::value << std::endl;
        throw "unknow type";
    }
}

#endif // TEST_MODEL_H
