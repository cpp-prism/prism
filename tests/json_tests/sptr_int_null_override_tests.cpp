#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("prismJson - shared_ptr<int> null override round trip", "[json][struct][shared_ptr][int][null]")
{
    SECTION("tst_struct my_sptr_int null overrides non-null default in JSON round trip")
    {
        // Default is make_shared<int>(2) - verify null overrides it
        tst_struct obj;
        obj.my_sptr_int = nullptr;

        std::string json = prism::json::toJsonString(obj);
        REQUIRE(json.find("\"my_sptr_int\":null") != std::string::npos);

        auto result = prism::json::fromJsonString<tst_struct>(json);
        REQUIRE(result->my_sptr_int == nullptr);
    }

    SECTION("tst_struct my_sptr_int explicit value round trip")
    {
        tst_struct obj;
        obj.my_sptr_int = std::make_shared<int>(42);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 42);
    }
}
