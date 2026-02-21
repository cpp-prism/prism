#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_shared_sub full fields round trip", "[yaml][flow][shared_ptr][substruct][full]")
{
    SECTION("my_shared_sub with all scalar fields flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 77;
        obj.my_shared_sub->my_bool = true;
        obj.my_shared_sub->my_string = "flow_shared_all";
        obj.my_shared_sub->my_longlong = 777777777LL;
        obj.my_shared_sub->my_opt_str = "flow_shared_opt";
        obj.my_shared_sub->my_opt_int = 99;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 77);
        REQUIRE(result->my_shared_sub->my_bool == true);
        REQUIRE(result->my_shared_sub->my_string == "flow_shared_all");
        REQUIRE(result->my_shared_sub->my_longlong == 777777777LL);
        REQUIRE(result->my_shared_sub->my_opt_str.has_value());
        REQUIRE(*result->my_shared_sub->my_opt_str == "flow_shared_opt");
        REQUIRE(result->my_shared_sub->my_opt_int.has_value());
        REQUIRE(*result->my_shared_sub->my_opt_int == 99);
    }

    SECTION("my_shared_sub with 2 my_vec_sp children flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 5;
        obj.my_shared_sub->my_string = "flow_shared_parent";

        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct child;
            child.my_int = i * 10;
            child.my_bool = (i == 1);
            child.my_string = "flow_child_" + std::to_string(i);
            obj.my_shared_sub->my_vec_sp.push_back(child);
        }

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 5);
        REQUIRE(result->my_shared_sub->my_string == "flow_shared_parent");
        REQUIRE(result->my_shared_sub->my_vec_sp.size() == 2);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_string == "flow_child_1");
        REQUIRE(result->my_shared_sub->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_shared_sub->my_vec_sp[1].my_bool == false);
        REQUIRE(result->my_shared_sub->my_vec_sp[1].my_string == "flow_child_2");
    }
}
