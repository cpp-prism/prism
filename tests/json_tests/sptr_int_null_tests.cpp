#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_sptr_int null and non-null with other fields round trip", "[json][sptr_int][null][combo]")
{
    SECTION("my_sptr_int null with bool true round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_bool = true;
        obj.my_sptr_int.reset();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_sptr_int == nullptr);
    }

    SECTION("my_sptr_int non-null negative value round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_sptr_int = std::make_shared<int>(-100);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == -100);
    }

    SECTION("my_sptr_int large positive value with string round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_string = "sptr_combo";
        obj.my_sptr_int = std::make_shared<int>(999999);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_string == "sptr_combo");
        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 999999);
    }
}
