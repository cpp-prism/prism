#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_sp elements with my_string field variations round trip", "[json][vec][string][field]")
{
    SECTION("my_vec_sp element with non-empty string round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "element_string_value";
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_string == "element_string_value");
    }

    SECTION("my_vec_sp element with empty string round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 20;
        sub.my_string = "";
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_string.empty());
    }

    SECTION("my_vec_sp multiple elements with different strings round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct s1;
        s1.my_int = 1;
        s1.my_string = "first_string";
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 2;
        s2.my_string = "second_unicode_中文";
        obj.my_vec_sp.push_back(s2);

        tst_sub_struct s3;
        s3.my_int = 3;
        s3.my_string = "";
        obj.my_vec_sp.push_back(s3);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 3);
        REQUIRE(result->my_vec_sp[0].my_string == "first_string");
        REQUIRE(result->my_vec_sp[1].my_string == "second_unicode_中文");
        REQUIRE(result->my_vec_sp[2].my_string.empty());
    }
}
