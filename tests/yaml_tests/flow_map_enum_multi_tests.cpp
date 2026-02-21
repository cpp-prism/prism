#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_map_enum with multiple entries round trip", "[yaml][flow][map][enum]")
{
    SECTION("my_map_enum with 3 entries flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_map_enum["en"] = language::english;
        obj.my_map_enum["zh_s"] = language::SimplifiedChinese;
        obj.my_map_enum["zh_t"] = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_map_enum.size() == 3);
        REQUIRE(result->my_map_enum["en"] == language::english);
        REQUIRE(result->my_map_enum["zh_s"] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["zh_t"] == language::TraditionalChinese);
    }

    SECTION("my_vec_enum with all 4 values flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::english, language::SimplifiedChinese, language::TraditionalChinese, language::unknow};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_vec_enum.size() == 4);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum[2] == language::TraditionalChinese);
        REQUIRE(result->my_vec_enum[3] == language::unknow);
    }
}
