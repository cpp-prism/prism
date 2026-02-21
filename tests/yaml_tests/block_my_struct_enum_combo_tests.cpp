#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_struct (embedded) combined with enum fields", "[yaml][block][embedded][enum][combo]")
{
    SECTION("my_struct.my_int + my_vec_enum + lang field block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 50;
        obj.lang = language::TraditionalChinese;
        obj.my_vec_enum = {language::english, language::unknow};

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 1);
        REQUIRE(result->my_struct.my_int == 50);
        REQUIRE(result->lang == language::TraditionalChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_vec_enum[0] == language::english);
        REQUIRE(result->my_vec_enum[1] == language::unknow);
    }

    SECTION("my_struct.my_vec_sp + my_map_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 100;

        tst_sub_struct sub;
        sub.my_int = 77;
        sub.my_string = "combo_sub";
        obj.my_struct.my_vec_sp.push_back(sub);

        obj.my_map_enum["en"] = language::english;
        obj.my_map_enum["zh"] = language::SimplifiedChinese;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 2);
        REQUIRE(result->my_struct.my_int == 100);
        REQUIRE(result->my_struct.my_vec_sp.size() == 1);
        REQUIRE(result->my_struct.my_vec_sp[0].my_int == 77);
        REQUIRE(result->my_struct.my_vec_sp[0].my_string == "combo_sub");
        REQUIRE(result->my_map_enum["en"] == language::english);
        REQUIRE(result->my_map_enum["zh"] == language::SimplifiedChinese);
    }

    SECTION("my_struct with opt fields + my_uptr_sub block round trip")
    {
        tst_struct obj;
        obj.my_int = 3;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_struct.my_int = 200;
        obj.my_struct.my_opt_str = "embedded_opt";

        obj.my_uptr_sub = std::make_unique<tst_sub_struct>();
        obj.my_uptr_sub->my_int = 55;
        obj.my_uptr_sub->my_string = "uptr_combo";

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 3);
        REQUIRE(result->my_struct.my_int == 200);
        REQUIRE(result->my_struct.my_opt_str.has_value());
        REQUIRE(*result->my_struct.my_opt_str == "embedded_opt");
        REQUIRE(result->my_uptr_sub != nullptr);
        REQUIRE(result->my_uptr_sub->my_int == 55);
        REQUIRE(result->my_uptr_sub->my_string == "uptr_combo");
    }
}
