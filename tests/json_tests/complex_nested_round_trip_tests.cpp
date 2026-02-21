#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - complex nested struct with multiple container types simultaneously", "[json][complex][nested][stress]")
{
    SECTION("tst_struct with my_map + my_vec_sp + my_shared_sub + my_vec_enum simultaneously round trip")
    {
        tst_struct obj;
        obj.my_int = 99;
        obj.my_bool = true;
        obj.my_string = "complex_root";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::TraditionalChinese;
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};
        obj.my_deque_int = {1, 2, 3};
        obj.my_set_str = {"x", "y"};

        // Add 2 elements to my_vec_sp
        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 100;
            sub.my_string = "vec_" + std::to_string(i);
            obj.my_vec_sp.push_back(sub);
        }

        // Set my_shared_sub
        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 500;
        obj.my_shared_sub->my_string = "shared_sub_complex";

        // Add 1 entry to my_map (with its own vec_sp)
        tst_struct map_val;
        map_val.my_int = 1000;
        map_val.my_list_int.clear();
        map_val.my_list_std_string.clear();
        tst_sub_struct map_sub;
        map_sub.my_int = 1001;
        map_sub.my_string = "map_child";
        map_val.my_vec_sp.push_back(map_sub);
        obj.my_map["complex_key"] = std::move(map_val);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 99);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "complex_root");
        REQUIRE(result->lang == language::TraditionalChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 100);
        REQUIRE(result->my_vec_sp[1].my_int == 200);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 500);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map["complex_key"].my_int == 1000);
        REQUIRE(result->my_map["complex_key"].my_vec_sp.size() == 1);
        REQUIRE(result->my_map["complex_key"].my_vec_sp[0].my_int == 1001);
    }
}
