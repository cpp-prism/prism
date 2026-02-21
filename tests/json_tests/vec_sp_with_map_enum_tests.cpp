#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_sp elements with my_map_enum populated round trip", "[json][vec][struct][enum][map]")
{
    SECTION("my_vec_sp element with my_map_enum round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_string = "sub_with_map_enum";
        obj.my_vec_sp.push_back(sub);

        obj.my_map_enum["en_key"] = language::english;
        obj.my_map_enum["zh_key"] = language::SimplifiedChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 5);
        REQUIRE(result->my_vec_sp[0].my_string == "sub_with_map_enum");
        REQUIRE(result->my_map_enum.size() == 2);
        REQUIRE(result->my_map_enum["en_key"] == language::english);
        REQUIRE(result->my_map_enum["zh_key"] == language::SimplifiedChinese);
    }

    SECTION("my_vec_sp 2 elements one with map_enum one without round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 10;
        obj.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 20;
        sub2.my_string = "second_sub";
        obj.my_vec_sp.push_back(sub2);

        obj.my_map_enum["tc_key"] = language::TraditionalChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_vec_sp[1].my_string == "second_sub");
        REQUIRE(result->my_map_enum.size() == 1);
        REQUIRE(result->my_map_enum["tc_key"] == language::TraditionalChinese);
    }
}
