#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_shared_self with container fields populated round trip", "[json][shared_self][containers][combo]")
{
    SECTION("my_shared_self with deque_int and set_str round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 11;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_deque_int = {10, 20};
        obj.my_shared_self->my_set_str = {"red", "blue"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 11);
        REQUIRE(result->my_shared_self->my_deque_int.size() == 2);
        REQUIRE(result->my_shared_self->my_set_str.count("red") == 1);
    }

    SECTION("my_shared_self with vec_sp populated round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 22;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 77;
        obj.my_shared_self->my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_shared_self->my_vec_sp.size() == 1);
        REQUIRE(result->my_shared_self->my_vec_sp[0].my_int == 77);
    }

    SECTION("my_shared_self null alongside containers on root round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self.reset();
        obj.my_deque_int = {5, 10, 15};
        obj.my_set_str = {"one", "two"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_shared_self == nullptr);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
    }
}
