#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_struct (embedded) all optional sub-fields round trip", "[yaml][flow][embedded][optional][struct]")
{
    SECTION("my_struct all optional fields populated flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 42;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "flow_embedded_full";
        obj.my_struct.my_longlong = 777777LL;
        obj.my_struct.my_opt_str = "flow_opt_val";
        obj.my_struct.my_opt_int = 66;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_struct.my_int == 42);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "flow_embedded_full");
        REQUIRE(result->my_struct.my_longlong == 777777LL);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "flow_opt_val");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 66);
    }

    SECTION("my_struct with 2 my_vec_sp elements flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 100;

        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            sub.my_bool = (i % 2 == 1);
            sub.my_string = "flow_struct_sub_" + std::to_string(i);
            obj.my_struct.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 2);
        for (int i = 0; i < 2; ++i)
        {
            REQUIRE(result->my_struct.my_vec_sp[i].my_int == (i + 1) * 10);
            REQUIRE(result->my_struct.my_vec_sp[i].my_bool == ((i + 1) % 2 == 1));
            REQUIRE(result->my_struct.my_vec_sp[i].my_string == "flow_struct_sub_" + std::to_string(i + 1));
        }
    }
}
