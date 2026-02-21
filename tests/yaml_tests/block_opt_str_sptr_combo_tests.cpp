#include "../models/test_model.h"
#include "../../include/prism/prismYaml.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismYaml - block format my_opt_str and my_sptr_int combo round trip", "[yaml][block][optional][sptr][combo]")
{
    SECTION("all optional/sptr fields populated block round trip")
    {
        tst_struct obj;
        obj.my_int = 1;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = "hello_opt";
        obj.my_sptr_int = std::make_shared<int>(99);

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(result->my_opt_str.has_value());
        REQUIRE(result->my_opt_str.value() == "hello_opt");
        REQUIRE(result->my_sptr_int != nullptr);
        REQUIRE(*result->my_sptr_int == 99);
    }

    SECTION("my_opt_str null, my_sptr_int null block round trip")
    {
        tst_struct obj;
        obj.my_int = 2;
        obj.my_list_int.clear();
        obj.my_list_std_string.clear();
        obj.my_opt_str = std::nullopt;
        obj.my_sptr_int.reset();

        std::string yaml = prism::yaml::toYamlStringBlock(obj);
        auto result = prism::yaml::fromYamlString<tst_struct>(yaml);

        REQUIRE(!result->my_opt_str.has_value());
        REQUIRE(result->my_sptr_int == nullptr);
    }
}
