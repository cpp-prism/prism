#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - my_struct (embedded tst_sub_struct) field combo round trip", "[json][my_struct][embedded][combo]")
{
    SECTION("my_struct with bool and string fields round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 10;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "embedded_val";
        obj.my_struct.my_longlong = 12345LL;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 10);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "embedded_val");
        REQUIRE(result->my_struct.my_longlong == 12345LL);
    }

    SECTION("my_struct with float and double fields round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 20;
        obj.my_struct.my_float = 1.25f;
        obj.my_struct.my_double = 3.14159;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 20);
        REQUIRE(result->my_struct.my_float == Catch::Approx(1.25f).epsilon(1e-4));
        REQUIRE(result->my_struct.my_double == Catch::Approx(3.14159).epsilon(1e-9));
    }

    SECTION("my_struct with opt fields round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 30;
        obj.my_struct.my_opt_str = "opt_embedded";
        obj.my_struct.my_opt_int = 77;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_struct.my_int == 30);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(result->my_struct.my_opt_str.value() == "opt_embedded");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(result->my_struct.my_opt_int.value() == 77);
    }
}
