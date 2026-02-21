#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - full tst_struct round trip with multiple fields", "[json][struct][round-trip]")
{
    SECTION("tst_struct with scalar fields round trip")
    {
        tst_struct original;
        original.my_int = 42;
        original.my_bool = true;
        original.my_float = 3.14f;
        original.my_double = 2.71828;
        original.my_longlong = 9876543210LL;
        original.my_string = "test string";
        original.my_opt_str = "optional str";
        original.my_opt_int = 99;
        original.lang = language::english;

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
        REQUIRE(std::abs(result->my_float - 3.14f) < 0.001f);
        REQUIRE(std::abs(result->my_double - 2.71828) < 0.00001);
        REQUIRE(result->my_longlong == 9876543210LL);
        REQUIRE(result->my_string == "test string");
        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "optional str");
        REQUIRE(result->my_opt_int.has_value());
        REQUIRE(*result->my_opt_int == 99);
        REQUIRE(result->lang == language::english);
    }

    SECTION("tst_struct with datetime fields round trip")
    {
        tst_struct original;
        original.my_datetime = initDatetime(2024, 3, 15, 10, 30, 0);
        original.my_timestamp = initTimestamp(2024, 3, 15, 10, 30, 0);

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        std::time_t orig_ts = std::chrono::system_clock::to_time_t(original.my_datetime);
        std::time_t res_ts = std::chrono::system_clock::to_time_t(result->my_datetime);
        REQUIRE(orig_ts == res_ts);
        REQUIRE(result->my_timestamp == original.my_timestamp);
    }

    SECTION("tst_struct with sptr_int field round trip")
    {
        tst_struct original;
        original.my_sptr_int = std::make_shared<int>(777);

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 777);
    }

    SECTION("tst_struct with list_int field round trip")
    {
        tst_struct original;
        original.my_list_int = {10, 20, 30, 40};
        original.my_list_std_string = {"a", "b", "c"};

        std::string json = prism::json::toJsonString(original);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_int.size() == 4);
        REQUIRE(result->my_list_std_string.size() == 3);
        auto it = result->my_list_int.begin();
        REQUIRE(*it++ == 10);
        REQUIRE(*it++ == 20);
        REQUIRE(*it++ == 30);
        REQUIRE(*it == 40);
        auto sit = result->my_list_std_string.begin();
        REQUIRE(*sit++ == "a");
        REQUIRE(*sit++ == "b");
        REQUIRE(*sit == "c");
    }
}
