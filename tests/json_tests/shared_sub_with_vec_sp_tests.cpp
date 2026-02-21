#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_shared_sub (shared_ptr<tst_sub_struct>) with my_vec_sp populated", "[json][struct][shared_ptr][vector]")
{
    SECTION("my_shared_sub with 2 my_vec_sp elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 10;
        obj.my_shared_sub->my_string = "shared_parent";

        tst_sub_struct child1;
        child1.my_int = 11;
        child1.my_string = "child_one";
        child1.my_bool = true;
        obj.my_shared_sub->my_vec_sp.push_back(child1);

        tst_sub_struct child2;
        child2.my_int = 22;
        child2.my_string = "child_two";
        child2.my_bool = false;
        obj.my_shared_sub->my_vec_sp.push_back(child2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 10);
        REQUIRE(result->my_shared_sub->my_string == "shared_parent");
        REQUIRE(result->my_shared_sub->my_vec_sp.size() == 2);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_int == 11);
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_string == "child_one");
        REQUIRE(result->my_shared_sub->my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_shared_sub->my_vec_sp[1].my_int == 22);
        REQUIRE(result->my_shared_sub->my_vec_sp[1].my_string == "child_two");
        REQUIRE(result->my_shared_sub->my_vec_sp[1].my_bool == false);
    }

    SECTION("my_vec_sp with 3 elements round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        for (int i = 1; i <= 3; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            sub.my_string = "element_" + std::to_string(i);
            sub.my_bool = (i % 2 == 0);
            obj.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_vec_sp.size() == 3);
        for (int i = 0; i < 3; ++i)
        {
            REQUIRE(result->my_vec_sp[i].my_int == (i + 1) * 10);
            REQUIRE(result->my_vec_sp[i].my_string == "element_" + std::to_string(i + 1));
            REQUIRE(result->my_vec_sp[i].my_bool == ((i + 1) % 2 == 0));
        }
    }
}
