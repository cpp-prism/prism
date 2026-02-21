#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_opt_str and my_sptr_int combo round trip", "[json][optional][sptr][combo]")
{
    SECTION("all optional/sptr fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "hello_opt";
        obj.my_sptr_int = std::make_shared<int>(99);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(result->my_opt_str.value() == "hello_opt");
        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 99);
    }

    SECTION("my_opt_str null, my_sptr_int null round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = std::nullopt;
        obj.my_sptr_int.reset();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_sptr_int == nullptr);
    }

    SECTION("my_opt_str empty string and my_sptr_int zero round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "";
        obj.my_sptr_int = std::make_shared<int>(0);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(result->my_opt_str.value().empty());
        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 0);
    }
}
