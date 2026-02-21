#include "../models/test_model.h"
#include "../../include/prism/prism.hpp"
#include <catch2/catch_test_macros.hpp>
#include <algorithm>

TEST_CASE("prism::enums - enum_info", "[reflection][enums]")
{
    SECTION("tostring")
    {
        REQUIRE(std::string(prism::enums::enum_info<language>::tostring(language::english)) == "英语");
        REQUIRE(std::string(prism::enums::enum_info<language>::tostring(language::SimplifiedChinese)) == "简体中文");
    }

    SECTION("fromstring")
    {
        REQUIRE(prism::enums::enum_info<language>::fromstring("英语") == language::english);
        REQUIRE(prism::enums::enum_info<language>::fromstring("简体中文") == language::SimplifiedChinese);
        REQUIRE(prism::enums::enum_info<language>::fromstring("未知") == language::unknow);
    }

    SECTION("get string vector")
    {
        auto vec = prism::enums::enum_info<language>::getStrVector();
        REQUIRE(vec.size() == 4);
        std::vector<std::string> svec(vec.begin(), vec.end());
        REQUIRE(std::find(svec.begin(), svec.end(), "未知") != svec.end());
        REQUIRE(std::find(svec.begin(), svec.end(), "英语") != svec.end());
        REQUIRE(std::find(svec.begin(), svec.end(), "简体中文") != svec.end());
        REQUIRE(std::find(svec.begin(), svec.end(), "瀪体中文") != svec.end());
    }
}
