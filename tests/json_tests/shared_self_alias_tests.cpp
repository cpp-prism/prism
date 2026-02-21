#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_shared_self recursive struct round trip", "[json][shared_self][recursive]")
{
    SECTION("my_shared_self null round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self.reset();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_self == nullptr);
    }

    SECTION("my_shared_self single level round trip")
    {
        tst_struct obj;
        obj.my_int = 10;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 20;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_shared_self.reset();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 10);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 20);
        REQUIRE(result->my_shared_self->my_shared_self == nullptr);
    }

    SECTION("my_shared_self with list_int populated round trip")
    {
        tst_struct obj;
        obj.my_int = 5;
        obj.my_list_int = {1, 2, 3};
        obj.my_list_std_string.clear();
        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 15;
        obj.my_shared_self->my_list_int = {4, 5};
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_shared_self.reset();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 5);
        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 15);
        REQUIRE(result->my_shared_self->my_list_int.size() == 2);
    }
}
