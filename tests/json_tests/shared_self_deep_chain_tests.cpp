#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_shared_self deep chain with different fields round trip", "[json][shared_ptr][self][chain]")
{
    SECTION("my_shared_self with my_vec_enum and nested child round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_vec_enum = {language::english};

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 100;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->lang = language::TraditionalChinese;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_enum.size() == 1);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 100);
        REQUIRE(result->my_shared_self->lang == language::TraditionalChinese);
    }

    SECTION("my_shared_self child with map and deque round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 200;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_deque_int = {7, 8, 9};

        tst_struct child_map_val;
        child_map_val.my_int = 300;
        child_map_val.my_list_int.clear();
        child_map_val.my_list_std_string.clear();
        child_map_val.my_string = "child_map_val";
        obj.my_shared_self->my_map["ck1"] = std::move(child_map_val);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_shared_self->my_int == 200);
        REQUIRE(result->my_shared_self->my_deque_int.size() == 3);
        REQUIRE(result->my_shared_self->my_deque_int[0] == 7);
        REQUIRE(result->my_shared_self->my_map.size() == 1);
        REQUIRE(result->my_shared_self->my_map["ck1"].my_int == 300);
        REQUIRE(result->my_shared_self->my_map["ck1"].my_string == "child_map_val");
    }
}
