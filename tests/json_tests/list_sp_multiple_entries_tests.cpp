#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_list_sp multiple entries with different field combos round trip", "[json][list][struct][combo]")
{
    SECTION("my_list_sp 3 entries each with different field states round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct e1;
        e1.my_int = 10;
        e1.my_list_int.clear();
        e1.my_list_std_string.clear();
        e1.my_string = "entry_one";
        e1.lang = language::english;
        obj.my_list_sp.push_back(std::move(e1));

        tst_struct e2;
        e2.my_int = 20;
        e2.my_list_int.clear();
        e2.my_list_std_string.clear();
        e2.my_bool = true;
        e2.my_vec_enum = {language::SimplifiedChinese};
        obj.my_list_sp.push_back(std::move(e2));

        tst_struct e3;
        e3.my_int = 30;
        e3.my_list_int.clear();
        e3.my_list_std_string.clear();
        e3.my_deque_int = {1, 2, 3};
        e3.my_set_str = {"a", "b"};
        obj.my_list_sp.push_back(std::move(e3));

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 3);
        auto it = result->my_list_sp.begin();
        REQUIRE(it->my_int == 10);
        REQUIRE(it->my_string == "entry_one");
        REQUIRE(it->lang == language::english);
        ++it;
        REQUIRE(it->my_int == 20);
        REQUIRE(it->my_bool == true);
        REQUIRE(it->my_vec_enum.size() == 1);
        REQUIRE(it->my_vec_enum[0] == language::SimplifiedChinese);
        ++it;
        REQUIRE(it->my_int == 30);
        REQUIRE(it->my_deque_int.size() == 3);
        REQUIRE(it->my_set_str.size() == 2);
    }

    SECTION("my_list_sp entries each with shared_sub round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 3; ++i)
        {
            tst_struct e;
            e.my_int = i * 100;
            e.my_list_int.clear();
            e.my_list_std_string.clear();
            e.my_shared_sub = std::make_shared<tst_sub_struct>();
            e.my_shared_sub->my_int = i * 1000;
            e.my_shared_sub->my_string = "sub_" + std::to_string(i);
            obj.my_list_sp.push_back(std::move(e));
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_list_sp.size() == 3);
        auto it = result->my_list_sp.begin();
        for (int i = 1; i <= 3; ++i, ++it)
        {
            REQUIRE(it->my_int == i * 100);
            REQUIRE(it->my_shared_sub != nullptr);
            REQUIRE(it->my_shared_sub->my_int == i * 1000);
            REQUIRE(it->my_shared_sub->my_string == "sub_" + std::to_string(i));
        }
    }
}
