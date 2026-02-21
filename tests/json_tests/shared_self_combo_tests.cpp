#include "../models/test_model.h"
#include "../../include/prism/prismJson.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismJson - my_shared_self with enum and container fields round trip", "[json][shared_ptr][recursive][enum][combo]")
{
    SECTION("my_shared_self child has my_vec_enum populated round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::english;

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 10;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_vec_enum = {language::SimplifiedChinese, language::TraditionalChinese};

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->lang == language::english);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 10);
        REQUIRE(result->my_shared_self->my_vec_enum.size() == 2);
        REQUIRE(result->my_shared_self->my_vec_enum[0] == language::SimplifiedChinese);
        REQUIRE(result->my_shared_self->my_vec_enum[1] == language::TraditionalChinese);
    }

    SECTION("my_shared_self child has my_uptr_sub and my_opt_struct round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        obj.my_shared_self = std::make_shared<tst_struct>();
        obj.my_shared_self->my_int = 20;
        obj.my_shared_self->my_list_int.clear();
        obj.my_shared_self->my_list_std_string.clear();
        obj.my_shared_self->my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_shared_self->my_uptr_sub->my_int = 55;
        obj.my_shared_self->my_uptr_sub->my_string = "child_uptr";

        tst_sub_struct opt_val;
        opt_val.my_int = 66;
        opt_val.my_string = "child_opt";
        obj.my_shared_self->my_opt_struct = opt_val;

        std::string json = prism::json::toJsonString(obj);
        auto result = prism::json::fromJsonString<tst_struct>(json);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_shared_self != nullptr);
        REQUIRE(result->my_shared_self->my_int == 20);
        REQUIRE(result->my_shared_self->my_uptr_sub != nullptr);
        REQUIRE(result->my_shared_self->my_uptr_sub->my_int == 55);
        REQUIRE(result->my_shared_self->my_uptr_sub->my_string == "child_uptr");
        REQUIRE(result->my_shared_self->my_opt_struct.has_value());
        REQUIRE(result->my_shared_self->my_opt_struct->my_int == 66);
        REQUIRE(result->my_shared_self->my_opt_struct->my_string == "child_opt");
    }
}
