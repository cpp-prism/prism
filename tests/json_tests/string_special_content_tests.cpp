#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_string with special content round trip", "[json][string][special]")
{
    SECTION("empty string round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_string = "";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string.empty());
    }

    SECTION("unicode string round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_string = "Hello 世界 🌍";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "Hello 世界 🌍");
    }

    SECTION("my_opt_str unicode string round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "中文测试 ñoño";

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(*result->my_opt_str == "中文测试 ñoño");
    }

    SECTION("vec_sp my_string with various special strings round trip")
    {
        tst_struct obj;
        obj.my_int = 4;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 1;
        sub1.my_string = "simple_string";
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 2;
        sub2.my_string = "string with spaces";
        obj.my_vec_sp.push_back(sub2);

        tst_sub_struct sub3;
        sub3.my_int = 3;
        sub3.my_string = "한국어";
        obj.my_vec_sp.push_back(sub3);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 3);
        REQUIRE(result->my_vec_sp[0].my_string == "simple_string");
        REQUIRE(result->my_vec_sp[1].my_string == "string with spaces");
        REQUIRE(result->my_vec_sp[2].my_string == "한국어");
    }
}
