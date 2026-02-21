#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismYaml - flow format my_map entries with bool and float/double fields round trip", "[yaml][flow][map][bool][float][double]")
{
    SECTION("my_map entry with bool and double (small value) flow round trip")
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
        mv.my_double = 0.005;
        obj.my_map["flow_entry"] = std::move(mv);

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["flow_entry"].my_bool == true);
        REQUIRE(result->my_map["flow_entry"].my_double == Catch::Approx(0.005).epsilon(1e-9));
    }
}
