#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_sp elements with my_vec_enum populated round trip", "[json][list][struct][enum][vector]")
{
    SECTION("my_list_sp element has my_vec_enum populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem;
        elem.my_int = 10;
        elem.my_list_int.clear();
        elem.my_list_std_string.clear();
        elem.my_vec_enum = {language::english, language::SimplifiedChinese};
        elem.lang = language::TraditionalChinese;
        obj.my_list_sp.push_back(std::move(elem));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_int == 10);
        REQUIRE(result->my_list_sp.front().lang == language::TraditionalChinese);
        REQUIRE(result->my_list_sp.front().my_vec_enum.size() == 2);
        REQUIRE(result->my_list_sp.front().my_vec_enum[0] == language::english);
        REQUIRE(result->my_list_sp.front().my_vec_enum[1] == language::SimplifiedChinese);
    }

    SECTION("my_list_sp with 2 elements having different enum configurations round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem1;
        elem1.my_int = 10;
        elem1.my_list_int.clear();
        elem1.my_list_std_string.clear();
        elem1.lang = language::english;
        elem1.my_vec_enum = {language::english};
        obj.my_list_sp.push_back(std::move(elem1));

        tst_struct elem2;
        elem2.my_int = 20;
        elem2.my_list_int.clear();
        elem2.my_list_std_string.clear();
        elem2.lang = language::SimplifiedChinese;
        elem2.my_vec_enum = {language::SimplifiedChinese, language::TraditionalChinese};
        obj.my_list_sp.push_back(std::move(elem2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->lang == language::english);
        REQUIRE(it->my_vec_enum.size() == 1);
        ++it;
        REQUIRE(it->my_int == 20);
        REQUIRE(it->lang == language::SimplifiedChinese);
        REQUIRE(it->my_vec_enum.size() == 2);
    }
}
