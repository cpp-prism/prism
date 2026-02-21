#include "../models/test_model.h"
#include "../../include/prism/prismSql.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("prismSql - INSERT", "[sql][insert]")
{
    SECTION("insert single model")
    {
        auto model = std::make_shared<database_table>();
        model->id = 1;
        model->guid = "2";
        model->myBool = true;
        model->myDouble = 5.23;

        std::string sql = prism::sql::sqlite3::Sqlite3::insert(std::vector<std::shared_ptr<database_table>>{model});
        REQUIRE(sql.find("INSERT INTO table_database_table") != std::string::npos);
        REQUIRE(sql.find("id, Uid, myBool, myDouble") != std::string::npos);
        REQUIRE(sql.find("VALUES") != std::string::npos);
        REQUIRE(sql.find("5.23") != std::string::npos);
        REQUIRE(sql.find("\"2\"") != std::string::npos);
    }

    SECTION("insert multiple models")
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
        REQUIRE(sql.find("INSERT INTO table_database_table") != std::string::npos);
        REQUIRE(sql.find("VALUES") != std::string::npos);
        // 验证实际字段值出现多次（3 个模型各有一组值）
        size_t occurrences = 0;
        size_t pos = 0;
        while ((pos = sql.find("5.23", pos)) != std::string::npos) {
            occurrences++;
            pos += 4;
        }
        REQUIRE(occurrences == 3);
    }
}
