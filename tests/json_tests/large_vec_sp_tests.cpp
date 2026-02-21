#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_vec_sp with many elements round trip", "[json][vec][large][stress]")
{
    SECTION("my_vec_sp with 10 elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 10; ++i) {
            tst_sub_struct s;
            s.my_int = i + 1;
            s.my_longlong = static_cast<long long>(i * 1000);
            s.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(s);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 10);
        for (int i = 0; i < 10; ++i) {
            REQUIRE(result->my_vec_sp[i].my_int == i + 1);
            REQUIRE(result->my_vec_sp[i].my_longlong == static_cast<long long>(i * 1000));
            REQUIRE(result->my_vec_sp[i].my_bool == (i % 2 == 0));
        }
    }

    SECTION("my_vec_sp with mixed string and numeric fields round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 0; i < 5; ++i) {
            tst_sub_struct s;
            s.my_int = i * 10;
            s.my_string = "item_" + std::to_string(i);
            obj.my_vec_sp.push_back(s);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_vec_sp.size() == 5);
        for (int i = 0; i < 5; ++i) {
            REQUIRE(result->my_vec_sp[i].my_int == i * 10);
            REQUIRE(result->my_vec_sp[i].my_string == "item_" + std::to_string(i));
        }
    }
}
