#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format all container types populated simultaneously stress round trip", "[yaml][flow][stress][containers]")
{
    SECTION("all containers populated simultaneously flow round trip")
    {
        tst_struct obj;
        obj.my_int = 88;
        obj.my_bool = false;
        obj.my_string = "flow_all_containers_stress";
        obj.my_list_int = {5, 6};
        obj.my_list_std_string = {"m", "n"};
        obj.my_deque_int = {-1, 0, 1};
        obj.my_set_str = {"s1", "s2"};
        obj.my_vec_enum = {language::SimplifiedChinese};
        obj.my_map_enum["fk"] = language::english;
        obj.lang = language::english;

        for (int i = 0; i < 2; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i + 1;
            sub.my_string = "flow_sub_" + std::to_string(i);
            obj.my_vec_sp.push_back(sub);
        }

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 777;
        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 888;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 88);
        REQUIRE(result->my_bool == false);
        REQUIRE(result->my_list_int.size() == 2);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_vec_enum.size() == 1);
        REQUIRE(result->my_vec_enum[0] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["fk"] == language::english);
        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 777);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 888);
    }
}
