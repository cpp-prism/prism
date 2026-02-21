#include "../models/test_model.h"
#include "../../include/prism/prismSql.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismSql - class alias", "[sql][class]")
{
    SECTION("get class alias")
    {
        auto attr = prism::attributes::getClassAttr<database_table, prism::sql::attributes::Attr_sql_class_alias>();
        REQUIRE(attr.has_value());
        REQUIRE(std::string(attr.value()) == "table_database_table");
    }

    SECTION("generate INSERT with class alias")
    {
        auto model = std::make_shared<database_table>();
        model->id = 1;
        model->guid = "test-uid";
        model->myBool = true;
        model->myDouble = 5.23;

        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::insert(std::vector<std::shared_ptr<database_table>>{model});
        REQUIRE(sql.find("INSERT INTO table_database_table") != std::string::npos);
    }

    SECTION("generate CREATE TABLE with class alias")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::createTable<database_table>();
        REQUIRE(sql.find("CREATE TABLE IF NOT EXISTS table_database_table") != std::string::npos);
    }
}
