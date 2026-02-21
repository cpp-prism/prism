#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_struct (embedded tst_sub_struct) nested combo round trip", "[yaml][flow][struct][nested][combo]")
{
    SECTION("my_struct with all scalar fields flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 42;
        obj.my_struct.my_string = "flow_embedded_scalar";
        obj.my_struct.my_opt_int = 7;
        obj.my_struct.my_opt_str = "opt_flow_embedded";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 42);
        REQUIRE(result->my_struct.my_string == "flow_embedded_scalar");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 7);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "opt_flow_embedded");
    }

    SECTION("my_struct with vec_sp alongside root vec_enum flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_struct.my_int = 100;
        tst_sub_struct child;
        child.my_int = 66;
        child.my_string = "flow_child_of_my_struct";
        obj.my_struct.my_vec_sp.push_back(child);

        obj.my_vec_enum = {language::SimplifiedChinese, language::TraditionalChinese};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 66);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[1] == language::TraditionalChinese);
    }
}
