#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format map<string,struct> value with populated sub-vector", "[yaml][block][map][vector][nested]")
{
    SECTION("tst_struct my_map entry whose my_vec_sp has 2 elements block round trip")
    {
        tst_struct obj;
        obj.my_int = 7;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct v;
        v.my_int = 10;
        v.my_list_int.clear();
        v.my_list_std_string.clear();

        tst_sub_struct sub1;
        sub1.my_int = 11;
        sub1.my_string = "sub_one";
        v.my_vec_sp.push_back(sub1);

        tst_sub_struct sub2;
        sub2.my_int = 22;
        sub2.my_string = "sub_two";
        v.my_vec_sp.push_back(sub2);

        obj.my_map["entry"] = std::move(v);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_int == 7);
        REQUIRE(result->my_map.size() == 1);
        REQUIRE(result->my_map.count("entry") == 1);

        auto& entry = result->my_map["entry"];
        REQUIRE(entry.my_int == 10);
        REQUIRE(entry.my_vec_sp.size() == 2);
        REQUIRE(entry.my_vec_sp[0].my_int == 11);
        REQUIRE(entry.my_vec_sp[0].my_string == "sub_one");
        REQUIRE(entry.my_vec_sp[1].my_int == 22);
        REQUIRE(entry.my_vec_sp[1].my_string == "sub_two");
    }

    SECTION("tst_struct my_map two entries each with one sub-struct in my_vec_sp block round trip")
    {
        tst_struct obj;
        obj.my_int = 8;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();

        tst_struct va;
        va.my_int = 100;
        va.my_list_int.clear();
        va.my_list_std_string.clear();
        tst_sub_struct suba;
        suba.my_int = 101;
        suba.my_bool = true;
        va.my_vec_sp.push_back(suba);
        obj.my_map["alpha"] = std::move(va);

        tst_struct vb;
        vb.my_int = 200;
        vb.my_list_int.clear();
        vb.my_list_std_string.clear();
        tst_sub_struct subb;
        subb.my_int = 201;
        subb.my_bool = false;
        vb.my_vec_sp.push_back(subb);
        obj.my_map["beta"] = std::move(vb);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_map.size() == 2);
        REQUIRE(result->my_map["alpha"].my_int == 100);
        REQUIRE(result->my_map["alpha"].my_vec_sp.size() == 1);
        REQUIRE(result->my_map["alpha"].my_vec_sp[0].my_int == 101);
        REQUIRE(result->my_map["alpha"].my_vec_sp[0].my_bool == true);
        REQUIRE(result->my_map["beta"].my_int == 200);
        REQUIRE(result->my_map["beta"].my_vec_sp.size() == 1);
        REQUIRE(result->my_map["beta"].my_vec_sp[0].my_int == 201);
        REQUIRE(result->my_map["beta"].my_vec_sp[0].my_bool == false);
    }
}
