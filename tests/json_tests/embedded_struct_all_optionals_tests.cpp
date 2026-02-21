#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_struct (embedded tst_sub_struct) all optional fields round trip", "[json][embedded][optional][struct]")
{
    SECTION("my_struct all optional fields populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 42;
        obj.my_struct.my_bool = true;
        obj.my_struct.my_string = "embedded_full";
        obj.my_struct.my_longlong = 999999LL;
        obj.my_struct.my_opt_str = "opt_str_val";
        obj.my_struct.my_opt_int = 77;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_struct.my_int == 42);
        REQUIRE(result->my_struct.my_bool == true);
        REQUIRE(result->my_struct.my_string == "embedded_full");
        REQUIRE(result->my_struct.my_longlong == 999999LL);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "opt_str_val");
        REQUIRE(result->my_struct.my_opt_int.has_value());
        REQUIRE(*result->my_struct.my_opt_int == 77);
    }

    SECTION("my_struct opt fields all null round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 5;
        obj.my_struct.my_string = "no_opts";
        // my_opt_str and my_opt_int remain nullopt

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_struct.my_int == 5);
        REQUIRE(result->my_struct.my_string == "no_opts");
        REQUIRE(!result->my_struct.my_opt_str.has_value());
        REQUIRE(!result->my_struct.my_opt_int.has_value());
    }

    SECTION("my_struct with 3 my_vec_sp elements round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 100;

        for (int i = 1; i <= 3; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            sub.my_bool = (i % 2 == 1);
            sub.my_string = "struct_sub_" + std::to_string(i);
            sub.my_opt_str = "opt_" + std::to_string(i);
            obj.my_struct.my_vec_sp.push_back(sub);
        }

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 3);
        for (int i = 0; i < 3; ++i)
        {
            REQUIRE(result->my_struct.my_vec_sp[i].my_int == (i + 1) * 10);
            REQUIRE(result->my_struct.my_vec_sp[i].my_bool == ((i + 1) % 2 == 1));
            REQUIRE(result->my_struct.my_vec_sp[i].my_string == "struct_sub_" + std::to_string(i + 1));
            REQUIRE(result->my_struct.my_vec_sp[i].my_opt_str.has_value());
            REQUIRE(*result->my_struct.my_vec_sp[i].my_opt_str == "opt_" + std::to_string(i + 1));
        }
    }
}
