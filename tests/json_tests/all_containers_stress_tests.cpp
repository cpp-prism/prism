#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - all container types populated simultaneously stress round trip", "[json][stress][containers]")
{
    SECTION("all containers populated simultaneously round trip")
    {
        tst_struct obj;
        obj.my_int = 99;
        obj.my_bool = true;
        obj.my_string = "all_containers_stress";
        obj.my_list_int = {1, 2, 3};
        obj.my_list_std_string = {"x", "y"};
        obj.my_deque_int = {10, 20};
        obj.my_set_str = {"p", "q"};
        obj.my_vec_enum = {language::english, language::TraditionalChinese};
        obj.my_map_enum["em1"] = language::SimplifiedChinese;
        obj.lang = language::TraditionalChinese;

        // vec_sp
        for (int i = 0; i < 3; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            obj.my_vec_sp.push_back(sub);
        }

        // my_map
        tst_struct mv;
        mv.my_int = 100;
        mv.my_list_int.clear();
        mv.my_list_std_string.clear();
        obj.my_map["mkey"] = std::move(mv);

        // my_unordermap
        tst_sub_struct uv;
        uv.my_int = 200;
        obj.my_unordermap["ukey"] = uv;

        // smart ptrs
        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 300;
        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 400;
        obj.my_opt_struct = tst_sub_struct{};
        obj.my_opt_struct->my_int = 500;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 99);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_map_enum.size() == 1);
        REQUIRE(result->lang == language::TraditionalChinese);
        REQUIRE(result->my_vec_sp.size() == 3);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_unordermap.size() == 1);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 300);
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 400);
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 500);
    }
}
