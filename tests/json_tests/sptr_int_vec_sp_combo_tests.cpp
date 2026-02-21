#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_sptr_int alongside vec_sp round trip", "[json][sptr_int][vec_sp][combo]")
{
    SECTION("my_sptr_int set and vec_sp two elements round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_sptr_int = std::make_shared<int>(42);

        tst_sub_struct s1;
        s1.my_int = 10;
        obj.my_vec_sp.push_back(s1);

        tst_sub_struct s2;
        s2.my_int = 20;
        obj.my_vec_sp.push_back(s2);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 42);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
    }

    SECTION("my_sptr_int null and vec_sp empty round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_sptr_int.reset();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_sptr_int == nullptr);
        REQUIRE(result->my_vec_sp.empty());
    }

    SECTION("my_sptr_int negative value and vec_sp one element round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_sptr_int = std::make_shared<int>(-99);

        tst_sub_struct sub;
        sub.my_int = 5;
        sub.my_bool = true;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(*result->my_sptr_int == -99);
        REQUIRE(result->my_vec_sp[0].my_bool == true);
    }
}
