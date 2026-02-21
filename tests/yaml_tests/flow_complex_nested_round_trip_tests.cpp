#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format complex nested struct with multiple containers", "[yaml][flow][complex][nested][stress]")
{
    SECTION("tst_struct with my_vec_sp + my_shared_sub + my_vec_enum + my_deque_int + my_set_str flow round trip")
    {
        tst_struct obj;
        obj.my_int = 88;
        obj.my_bool = true;
        obj.my_string = "flow_complex_root";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::SimplifiedChinese;
        obj.my_vec_enum = {language::english, language::TraditionalChinese};
        obj.my_deque_int = {100, 200};
        obj.my_set_str = {"aa", "bb", "cc"};

        // Add 2 elements to my_vec_sp
        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 10;
            sub.my_string = "flow_vec_" + std::to_string(i);
            obj.my_vec_sp.push_back(sub);
        }

        // Set my_shared_sub with child
        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 999;
        obj.my_shared_sub->my_string = "flow_shared_complex";

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 88);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->lang == language::SimplifiedChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_deque_int.size() == 2);
        REQUIRE(result->my_set_str.size() == 3);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 10);
        REQUIRE(result->my_vec_sp[1].my_int == 20);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 999);
        REQUIRE(result->my_shared_sub->my_string == "flow_shared_complex");
    }
}
