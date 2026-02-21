#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_struct all optional sub-fields round trip", "[yaml][block][embedded][optional][struct]")
{
    SECTION("my_struct all optional fields populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 42;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "block_embedded_full";
        obj.my_struct.my_longlong = 888888LL;
        obj.my_struct.my_opt_str = "block_opt_val";
        obj.my_struct.my_opt_int = 55;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_struct.my_int == 42);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "block_embedded_full");
        REQUIRE(result->my_struct.my_longlong == 888888LL);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "block_opt_val");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 55);
    }

    SECTION("my_struct with 3 my_vec_sp elements including opt_str block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 200;

        for (int i = 1; i <= 3; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            sub.my_bool = (i % 2 == 0);
            sub.my_string = "block_struct_sub_" + std::to_string(i);
            sub.my_opt_str = "block_opt_" + std::to_string(i);
            obj.my_struct.my_vec_sp.push_back(sub);
        }

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_struct.my_int == 200);
        REQUIRE(result->my_struct.my_vec_sp.size() == 3);
        for (int i = 0; i < 3; ++i)
        {
            REQUIRE(result->my_struct.my_vec_sp[i].my_int == (i + 1) * 10);
            REQUIRE(result->my_struct.my_vec_sp[i].my_bool == ((i + 1) % 2 == 0));
            REQUIRE(result->my_struct.my_vec_sp[i].my_string == "block_struct_sub_" + std::to_string(i + 1));
            REQUIRE(result->my_struct.my_vec_sp[i].my_opt_str.has_value());
            REQUIRE(*result->my_struct.my_vec_sp[i].my_opt_str == "block_opt_" + std::to_string(i + 1));
        }
    }
}
