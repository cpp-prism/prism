#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format complex nested struct with multiple containers", "[yaml][block][complex][nested][stress]")
{
    SECTION("tst_struct with my_map + my_vec_sp + my_shared_sub + my_vec_enum block round trip")
    {
        tst_struct obj;
        obj.my_int = 99;
        obj.my_bool = true;
        obj.my_string = "block_complex_root";
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.lang = language::TraditionalChinese;
        obj.my_vec_enum = {language::english, language::SimplifiedChinese};
        obj.my_deque_int = {10, 20, 30};
        obj.my_set_str = {"p", "q"};

        // Add 2 elements to my_vec_sp
        for (int i = 1; i <= 2; ++i)
        {
            tst_sub_struct sub;
            sub.my_int = i * 100;
            sub.my_string = "block_vec_" + std::to_string(i);
            obj.my_vec_sp.push_back(sub);
        }

        // Set my_shared_sub
        obj.my_shared_sub = std::make_shared<tst_sub_struct>();
        obj.my_shared_sub->my_int = 500;
        obj.my_shared_sub->my_string = "block_shared_complex";

        // Add 1 entry to my_unordermap
        tst_sub_struct umap_val;
        umap_val.my_int = 777;
        umap_val.my_string = "block_umap_val";
        obj.my_unordermap["umap_key"] = umap_val;

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 99);
        REQUIRE(result->my_bool == true);
        REQUIRE(result->my_string == "block_complex_root");
        REQUIRE(result->lang == language::TraditionalChinese);
        REQUIRE(result->my_vec_enum.size() == 2);
        REQUIRE(result->my_deque_int.size() == 3);
        REQUIRE(result->my_set_str.size() == 2);
        REQUIRE(result->my_vec_sp.size() == 2);
        REQUIRE(result->my_vec_sp[0].my_int == 100);
        REQUIRE(result->my_vec_sp[1].my_int == 200);
        REQUIRE(result->my_shared_sub != nullptr);
        REQUIRE(result->my_shared_sub->my_int == 500);
        REQUIRE(result->my_shared_sub->my_string == "block_shared_complex");
        REQUIRE(result->my_unordermap["umap_key"].my_int == 777);
        REQUIRE(result->my_unordermap["umap_key"].my_string == "block_umap_val");
    }
}
