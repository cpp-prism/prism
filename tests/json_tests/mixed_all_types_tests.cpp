#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

TEST_CASE("prismJson - tst_struct all major field types populated together round trip", "[json][all_types][combo]")
{
    SECTION("all major types set simultaneously round trip")
    {
        tst_struct obj;
        obj.my_int = 42;
        obj.my_bool = true;
        obj.my_float = 1.5f;
        obj.my_double = 3.14;
        obj.my_string = "full_test";
        obj.my_opt_int = 99;
        obj.my_opt_str = "opt_val";
        obj.my_sptr_int = std::make_shared<int>(7);
        obj.lang = language::english;
        obj.my_list_int = {1, 2, 3};
        obj.my_list_std_string = {"a", "b"};
        obj.my_deque_int = {10, 20};
        obj.my_set_str = {"x", "y"};
        obj.my_vec_enum = {language::SimplifiedChinese};
        obj.my_map_enum["k"] = language::unknow;

        tst_sub_struct sub;
        sub.my_int = 100;
        obj.my_vec_sp.push_back(sub);

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 42);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_float == Catch::Approx(1.5f));
        REQUIRE(result->my_string == "full_test");
        REQUIRE(*result->my_opt_int == 99);
        REQUIRE(*result->my_opt_str == "opt_val");
        REQUIRE(*result->my_sptr_int == 7);
        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_list_int.size() == 3);
        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_set_str.count("x") == 1);
        REQUIRE(result->my_vec_enum[0] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum.at("k") == language::unknow);
        REQUIRE(result->my_vec_sp[0].my_int == 100);
    }

    SECTION("same struct but all optional/nullable fields are null round trip")
    {
        tst_struct obj;
        obj.my_int = 0;
        obj.my_bool = false;
        obj.my_float = 0.0f;
        obj.my_double = 0.0;
        obj.my_string = "";
        obj.my_opt_int = std::nullopt;
        obj.my_opt_str = std::nullopt;
        obj.my_sptr_int.reset();
        obj.lang = language::unknow;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_deque_int.clear();
        obj.my_set_str.clear();
        obj.my_vec_enum.clear();
        obj.my_map_enum.clear();

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 0);
        REQUIRE(!result->my_opt_int.has_value());
        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_sptr_int == nullptr);
        REQUIRE(result->my_list_int.empty());
        REQUIRE(result->my_vec_enum.empty());
    }
}
