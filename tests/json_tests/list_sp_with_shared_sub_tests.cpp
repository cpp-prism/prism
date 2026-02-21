#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_sp elements with my_shared_sub populated round trip", "[json][list][shared_ptr][struct]")
{
    SECTION("my_list_sp element has my_shared_sub non-null round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem;
        elem.my_int = 10;
        elem.my_list_int.clear();
        elem.my_list_std_string.clear();
        elem.my_shared_sub = std::make_shared<tst_sub_struct>();
        elem.my_shared_sub->my_int = 999;
        elem.my_shared_sub->my_string = "nested_shared_sub";
        obj.my_list_sp.push_back(std::move(elem));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 1);
        REQUIRE(result->my_list_sp.front().my_int == 10);
        REQUIRE(result->my_list_sp.front().my_shared_sub != nullptr);
        REQUIRE(result->my_list_sp.front().my_shared_sub->my_int == 999);
        REQUIRE(result->my_list_sp.front().my_shared_sub->my_string == "nested_shared_sub");
    }

    SECTION("my_list_sp 2 elements one with shared_sub one without round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct elem1;
        elem1.my_int = 20;
        elem1.my_list_int.clear();
        elem1.my_list_std_string.clear();
        elem1.my_shared_sub = std::make_shared<tst_sub_struct>();
        elem1.my_shared_sub->my_int = 555;
        obj.my_list_sp.push_back(std::move(elem1));

        tst_struct elem2;
        elem2.my_int = 30;
        elem2.my_list_int.clear();
        elem2.my_list_std_string.clear();
        // no shared_sub
        obj.my_list_sp.push_back(std::move(elem2));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 2);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_shared_sub != nullptr);
        REQUIRE(it->my_shared_sub->my_int == 555);
        ++it;
        REQUIRE(it->my_int == 30);
        REQUIRE(it->my_shared_sub == nullptr);
    }
}
