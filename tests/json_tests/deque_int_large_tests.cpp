#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_deque_int large sizes and mixed values round trip", "[json][deque][large]")
{
    SECTION("my_deque_int 15 elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 15; ++i) {
            obj.my_deque_int.push_back(i * 3);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 15);
        REQUIRE(result->my_deque_int[0] == 3);
        REQUIRE(result->my_deque_int[14] == 45);
    }

    SECTION("my_deque_int mixed negative, zero, positive round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {-100, -50, 0, 50, 100};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 5);
        REQUIRE(result->my_deque_int[0] == -100);
        REQUIRE(result->my_deque_int[2] == 0);
        REQUIRE(result->my_deque_int[4] == 100);
    }

    SECTION("my_deque_int single element round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int = {999};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_deque_int.size() == 1);
        REQUIRE(result->my_deque_int[0] == 999);
    }
}
