#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - multiple field combinations in single struct round trip", "[json][combo][struct]")
{
    SECTION("my_vec_sp + my_deque_int + my_set_str populated simultaneously round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_sub_struct sub;
        sub.my_int = 10;
        sub.my_string = "combo_sub";
        obj.my_vec_sp.push_back(sub);

        obj.my_deque_int = {100, 200, 300};
        obj.my_set_str = {"alpha", "beta", "gamma"};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_vec_sp.size() == 1);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[0].my_string == "combo_sub");
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_deque_int[0] == 100);
        REQUIRE(result->my_deque_int[1] == 200);
        REQUIRE(result->my_deque_int[2] == 300);
        REQUIRE(result->my_set_str.count("alpha") == 1);
        REQUIRE(result->my_set_str.count("beta") == 1);
        REQUIRE(result->my_set_str.count("gamma") == 1);
    }

    SECTION("my_shared_sub + my_uptr_sub + my_opt_struct all populated round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 11;
        obj.my_shared_sub->my_string = "shared_combo";

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 22;
        obj.my_uptr_sub->my_string = "uptr_combo";

        tst_sub_struct opt_val;
        opt_val.my_int = 33;
        opt_val.my_string = "opt_combo";
        obj.my_opt_struct = opt_val;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 11);
        REQUIRE(result->my_shared_sub->my_string == "shared_combo");
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 22);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_combo");
        REQUIRE(result->my_opt_struct.has_value());
        REQUIRE(result->my_opt_struct->my_int == 33);
        REQUIRE(result->my_opt_struct->my_string == "opt_combo");
    }

    SECTION("my_vec_enum + my_map_enum + lang field simultaneously round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::TraditionalChinese;
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};
        obj.my_map_enum["zh"] = language::SimplifiedChinese;
        obj.my_map_enum["en"] = language::english;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->lang == language::TraditionalChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["zh"] == language::SimplifiedChinese);
        REQUIRE(result->my_map_enum["en"] == language::english);
    }
}
