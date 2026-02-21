#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - my_map entries with bool, float, double fields round trip", "[json][map][bool][float][double]")
{
    SECTION("my_map entry with my_bool and my_float round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct mv;
        mv.my_int = 10;
        mv.my_list_int.clear();
        mv.my_list_std_string.clear();
        mv.my_bool = true;
        mv.my_float = 2.5f;
        mv.my_double = 0.001;
        obj.my_map["float_entry"] = std::move(mv);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["float_entry"].my_bool == true);
        REQUIRE(result->my_map["float_entry"].my_float == Catch::Approx(2.5f).epsilon(1e-5));
        REQUIRE(result->my_map["float_entry"].my_double == Catch::Approx(0.001).epsilon(1e-9));
    }

    SECTION("my_map 2 entries with mixed bool and float values round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct mv1;
        mv1.my_int = 10;
        mv1.my_list_int.clear();
        mv1.my_list_std_string.clear();
        mv1.my_bool = true;
        mv1.my_double = 3.14;
        obj.my_map["pi"] = std::move(mv1);

        tst_struct mv2;
        mv2.my_int = 20;
        mv2.my_list_int.clear();
        mv2.my_list_std_string.clear();
        mv2.my_bool = false;
        mv2.my_double = -2.718;
        obj.my_map["e"] = std::move(mv2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_map["pi"].my_bool == true);
        REQUIRE(result->my_map["pi"].my_double == Catch::Approx(3.14).epsilon(1e-9));
        REQUIRE(result->my_map["e"].my_bool == false);
        REQUIRE(result->my_map["e"].my_double == Catch::Approx(-2.718).epsilon(1e-9));
    }
}
