#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_list_sp elements with enum fields round trip", "[yaml][flow][list][struct][enum]")
{
    SECTION("my_list_sp element has my_vec_enum and lang populated flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem;
        elem.my_int = 10;
        elem.my_list_int.clear();
        elem.my_list_std_string.clear();
        elem.lang = language::TraditionalChinese;
        elem.my_vec_enum = {language::english, language::SimplifiedChinese};
        obj.my_list_sp.push_back(std::move(elem));

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_int == 10);
        REQUIRE(result->my_list_sp.front().lang == language::TraditionalChinese);
        REQUIRE(result->my_list_sp.front().my_vec_enum.size() == 2);
        REQUIRE(result->my_list_sp.front().my_vec_enum[0] == language::english);
        REQUIRE(result->my_list_sp.front().my_vec_enum[1] == language::SimplifiedChinese);
    }
}
