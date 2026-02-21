#include "../models/test_model.h"
#include "../../include/prism/prismSql.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismSql - SELECT", "[sql][select]")
{
    SECTION("generate SELECT all")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::queryTable<database_table>();
        REQUIRE(sql.find("SELECT") != std::string::npos);
        REQUIRE(sql.find("FROM table_database_table") != std::string::npos);
        // 验证具体列名出现
        REQUIRE(sql.find("id") != std::string::npos);
        REQUIRE(sql.find("Uid") != std::string::npos);
        REQUIRE(sql.find("myBool") != std::string::npos);
        REQUIRE(sql.find("myDouble") != std::string::npos);
        // 验证被 ignore 的字段不出现
        REQUIRE(sql.find("myFloat") == std::string::npos);
    }

    SECTION("generate SELECT with where")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::queryTable<database_table>("WHERE id = 1");
        REQUIRE(sql.find("WHERE id = 1") != std::string::npos);
    }
}
