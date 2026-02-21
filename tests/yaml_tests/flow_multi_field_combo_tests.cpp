#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format multiple field combinations round trip", "[yaml][flow][combo][struct]")
{
    SECTION("my_vec_sp + my_deque_int + my_set_str simultaneously flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "flow_combo_sub";
        obj.my_vec_sp.push_back(sub);

        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"alpha", "beta"};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_string == "flow_combo_sub");
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 100);
        REQUIRE(result->my_set_str.count("alpha") == 1);
        REQUIRE(result->my_set_str.count("beta") == 1);
    }

    SECTION("my_shared_sub + my_uptr_sub + my_opt_struct all populated flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 11;
        obj.my_shared_sub->my_string = "flow_shared_combo";

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 22;
        obj.my_uptr_sub->my_string = "flow_uptr_combo";

        tst_sub_struct opt_val;
        opt_val.my_int = 33;
        opt_val.my_string = "flow_opt_combo";
        obj.my_opt_struct = opt_val;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 11);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 22);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 33);
    }

    SECTION("my_vec_enum + lang + my_map_enum flow round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::SimplifiedChinese;
        obj.my_vec_enum = {language::english, language::unknow};
        obj.my_map_enum["a"] = language::TraditionalChinese;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_map_enum["a"] == language::TraditionalChinese);
    }
}
