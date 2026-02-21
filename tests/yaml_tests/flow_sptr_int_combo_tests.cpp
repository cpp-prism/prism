#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - flow format my_sptr_int combined with other fields round trip", "[yaml][flow][shared_ptr][int][combo]")
{
    SECTION("my_sptr_int overridden value with string and bool flow round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_sptr_int = std::make_shared<int>(77);
        obj.my_string = "flow_sptr_combo";
        obj.my_bool = true;

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 77);
        REQUIRE(result->my_string == "flow_sptr_combo");
        REQUIRE(result->my_bool == true);
    }

    SECTION("my_sptr_int null with deque flow round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_sptr_int = nullptr;
        obj.my_deque_int = {1, 2, 3};

        std::string yaml = prism::yaml::toYamlStringFlow(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_sptr_int == nullptr);
        REQUIRE(result->my_deque_int.size() == 3);
    }
}
