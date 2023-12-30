#define CATCH_CONFIG_RUNNER
#include "../prism/include/prism/prismJson.hpp"
#include "models/test_model.h"
#include <catch2/catch_all.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif
TEST_CASE("json测试")
{
    tst_struct instance_1;
    instance_1.my_int = 1;
    instance_1.my_bool = true;
    instance_1.my_float = 1.12345678901234567890f;
    instance_1.my_double = 2.12345678901234567890;
    instance_1.my_string = "333";
    instance_1.my_longlong = 4444;
    instance_1.my_opt_str = std::make_optional<std::string>("hehe");
    instance_1.my_opt_int = 3;
    instance_1.my_ptr_int = new int(4);
    instance_1.my_opt_ptr = std::make_optional<bool*>(new bool(true));
    instance_1.my_timestamp = initTimestamp(2023, 6, 23, 12, 00, 00);
    instance_1.my_datetime = initDatetime(2023, 6, 23, 12, 12, 34);
    instance_1.my_map["hehe111"] = {instance_1};
    instance_1.my_shared_sub = std::make_shared<tst_sub_struct>();
    instance_1.my_shared_self = std::make_shared<tst_struct>(instance_1);
    instance_1.my_vec_sp = {{}, {}};
    instance_1.my_vec_sp[0].my_vec_sp = {{}, {}};
    instance_1.my_vec_sp[1].my_vec_sp = {{}, {}};
    instance_1.my_list_sp = {instance_1, instance_1};
    instance_1.my_map["hehe111"] = {};
    instance_1.my_map["hehe112"] = {};
    instance_1.my_map["hehe113"] = {};

    /**
     *  Because unordermap is unordered,
     *  it will cause json string comparisons to be unequal,
     *  so comment out these three lines of code,
     *   but it does not mean that unordermap is not supported
     **/
    // instance_1.my_unordermap["hehe114"] = {};
    // instance_1.my_unordermap["hehe114"] = {};
    // instance_1.my_unordermap["hehe115"] = {};

    std::string json;
    std::shared_ptr<tst_struct> t;
    // BENCHMARK("serialization  100 times")
    //{
    json = prism::json::toJsonString(std::move(instance_1), 2);
    //};

    // BENCHMARK("deserialization 100 times")
    //{
    t = prism::json::fromJsonString<tst_struct>(std::move(json));
    //};
    std::string json2 = prism::json::toJsonString(std::move(*t), 2);

    // std::cout << json << std::endl;
    std::cout << json2 << std::endl;
    // return;

    SECTION("struct.member")
    {
        CHECK(instance_1.my_bool == t->my_bool);
        CHECK(instance_1.my_datetime == t->my_datetime);
        CHECK(std::abs(instance_1.my_double - t->my_double) < 0.0001);
        CHECK(std::abs(instance_1.my_float - t->my_float) < 0.0001);
        CHECK(instance_1.my_int == t->my_int);
        CHECK(instance_1.my_longlong == t->my_longlong);
        CHECK(instance_1.my_opt_int == t->my_opt_int);
        CHECK(instance_1.my_opt_str == t->my_opt_str);

        if (instance_1.my_sptr_int != nullptr && t->my_sptr_int != nullptr)
            CHECK(*instance_1.my_sptr_int == *t->my_sptr_int);
        else
        {
            CHECK(instance_1.my_sptr_int == nullptr);
            CHECK(t->my_sptr_int == nullptr);
        }

        CHECK(instance_1.my_string == t->my_string);
        CHECK(instance_1.my_timestamp == t->my_timestamp);
        CHECK(instance_1.my_list_int.size() == t->my_list_int.size());
        CHECK(instance_1.my_list_int.front() == t->my_list_int.front());
        CHECK(instance_1.my_list_std_string.size() == t->my_list_std_string.size());
        CHECK(instance_1.my_list_std_string.front() == t->my_list_std_string.front());
        CHECK(instance_1.lang == t->lang);
    }

    SECTION("struct.struct.member")
    {
        CHECK(instance_1.my_struct.my_bool == t->my_struct.my_bool);
        CHECK(std::abs(instance_1.my_struct.my_double - t->my_struct.my_double) < 0.0001);
        CHECK(std::abs(instance_1.my_struct.my_float - t->my_struct.my_float) < 0.0001);
        CHECK(instance_1.my_struct.my_int == t->my_struct.my_int);
        CHECK(instance_1.my_struct.my_longlong == t->my_struct.my_longlong);
        CHECK(instance_1.my_struct.my_opt_int == t->my_struct.my_opt_int);
        CHECK(instance_1.my_struct.my_opt_ptr == t->my_struct.my_opt_ptr);
        CHECK(instance_1.my_struct.my_opt_str == t->my_struct.my_opt_str);
        CHECK(instance_1.my_struct.my_ptr_int == t->my_struct.my_ptr_int);
        CHECK(instance_1.my_struct.my_string == t->my_struct.my_string);
    }

    SECTION("struct.list<struct>.member")
    {
        CHECK(instance_1.my_list_sp.front().my_bool == t->my_list_sp.front().my_bool);
        CHECK(instance_1.my_list_sp.front().my_datetime == t->my_list_sp.front().my_datetime);
        CHECK(std::abs(instance_1.my_list_sp.front().my_double - t->my_list_sp.front().my_double) < 0.0001);
        CHECK(std::abs(instance_1.my_list_sp.front().my_float - t->my_list_sp.front().my_float) < 0.0001);
        CHECK(instance_1.my_list_sp.front().my_int == t->my_list_sp.front().my_int);
        CHECK(instance_1.my_list_sp.front().my_longlong == t->my_list_sp.front().my_longlong);
        CHECK(instance_1.my_list_sp.front().my_opt_int == t->my_list_sp.front().my_opt_int);

        if (instance_1.my_list_sp.front().my_opt_ptr.has_value() && t->my_list_sp.front().my_opt_ptr.has_value())
            CHECK(*instance_1.my_list_sp.front().my_opt_ptr.value() == *t->my_list_sp.front().my_opt_ptr.value());
        else
        {
            CHECK(instance_1.my_list_sp.front().my_opt_ptr.has_value() == false);
            CHECK(t->my_list_sp.front().my_opt_ptr.has_value() == false);
        }

        CHECK(instance_1.my_list_sp.front().my_opt_str == t->my_list_sp.front().my_opt_str);
        CHECK(*instance_1.my_list_sp.front().my_ptr_int == *t->my_list_sp.front().my_ptr_int);
        CHECK(*instance_1.my_list_sp.front().my_sptr_int == *t->my_list_sp.front().my_sptr_int);
        if (instance_1.my_list_sp.front().my_sptr_int != nullptr && t->my_list_sp.front().my_sptr_int != nullptr)
            CHECK(*instance_1.my_list_sp.front().my_sptr_int == *t->my_list_sp.front().my_sptr_int);
        else
        {
            CHECK(instance_1.my_list_sp.front().my_sptr_int == nullptr);
            CHECK(t->my_list_sp.front().my_sptr_int == nullptr);
        }
        CHECK(instance_1.my_list_sp.front().my_string == t->my_list_sp.front().my_string);
        CHECK(instance_1.my_list_sp.front().my_timestamp == t->my_list_sp.front().my_timestamp);
        CHECK(instance_1.my_list_sp.front().lang == t->my_list_sp.front().lang);
    }

    SECTION("struct.shared_ptr<struct>->member")
    {
        CHECK(instance_1.my_shared_sub->my_bool == t->my_shared_sub->my_bool);
        CHECK(instance_1.my_shared_sub->my_double == t->my_shared_sub->my_double);
        CHECK(instance_1.my_shared_sub->my_float == t->my_shared_sub->my_float);
        CHECK(instance_1.my_shared_sub->my_int == t->my_shared_sub->my_int);
        CHECK(instance_1.my_shared_sub->my_opt_int == t->my_shared_sub->my_opt_int);
        CHECK(instance_1.my_shared_sub->my_opt_ptr == t->my_shared_sub->my_opt_ptr);
        CHECK(instance_1.my_shared_sub->my_opt_str == t->my_shared_sub->my_opt_str);
        CHECK(instance_1.my_shared_sub->my_ptr_int == t->my_shared_sub->my_ptr_int);
        CHECK(instance_1.my_shared_sub->my_string == t->my_shared_sub->my_string);
        CHECK(instance_1.my_shared_sub->my_longlong == t->my_shared_sub->my_longlong);
    }
    REQUIRE(json2 == json);
}

int main(int argc, const char** argv)
{
#ifdef _MSC_VER
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int result = Catch::Session().run(argc, argv);
    return result;
}
