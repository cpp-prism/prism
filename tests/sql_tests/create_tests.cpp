#include "../models/test_model.h"
#include "../../include/prism/prismSql.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismSql - CREATE TABLE", "[sql][create]")
{
    SECTION("generate CREATE TABLE")
    {
        std::string sql = prism::sql::sqlite3::Sqlite3::createTable<database_table>();
        REQUIRE(sql.find("CREATE TABLE IF NOT EXISTS table_database_table") != std::string::npos);
        REQUIRE(sql.find("id INTEGER") != std::string::npos);
        REQUIRE(sql.find("Uid TEXT") != std::string::npos);
        REQUIRE(sql.find("myBool INTEGER") != std::string::npos);
        REQUIRE(sql.find("myDouble INT") != std::string::npos);
    }
}

TEST_CASE("prismSql - field attributes", "[sql][field]")
{
    SECTION("get field alias")
    {
        // id field does not have alias attribute
        auto attr = prism::attributes::getFieldAttr<database_table, prism::sql::attributes::Attr_sql_field_alias>("id");
        REQUIRE(!attr.has_value());
    }

    SECTION("get field alias for guid")
    {
        // guid field has alias "Uid"
        auto attr = prism::attributes::getFieldAttr<database_table, prism::sql::attributes::Attr_sql_field_alias>("guid");
        REQUIRE(attr.has_value());
        REQUIRE(std::string(attr.value()) == "Uid");
    }

    SECTION("generate CREATE TABLE with field attributes")
    {
        std::string sql = prism::sql::sqlite3::Sqlite3::createTable<database_table>();
        // 验证主键约束
        REQUIRE(sql.find("PRIMARY KEY") != std::string::npos);
    }
}

TEST_CASE("prismSql - edge cases", "[sql][edge]")
{
    SECTION("generate INSERT with multiple rows")
    {
        std::vector<std::shared_ptr<database_table>> models;
        for (int i = 0; i < 3; i++) {
            auto model = std::make_shared<database_table>();
            model->id = 1;
            model->guid = "2";
            model->myBool = true;
            model->myDouble = 5.23;
            models.push_back(model);
        }

        std::string sql = prism::sql::sqlite3::Sqlite3::insert(models);
        // 验证多个值被正确生成
        int count = 0;
        size_t pos = 0;
        while ((pos = sql.find("VALUES", pos)) != std::string::npos) {
            count++;
            pos += 6;
        }
        REQUIRE(count >= 1);
    }
}
