#define CATCH_CONFIG_RUNNER

#include "../include/prism/prism.hpp"
#include "models/test_model.h"
#include <catch2/catch_all.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <type_traits>

#include "../include/prism/prismSql.hpp"
#include <iostream>
#include <stdio.h>

#ifdef _MSC_VER
#include <Windows.h>
#endif

struct database_table
{
    int id = 1;
    std::string guid = "2";
    bool myBool = true;
    float myFloat = 4.1;
    double myDouble = 5.230;
};

PRISM_FIELDS(database_table, id, guid, myBool, myFloat, myDouble);

// sql表别名,不用这个宏标注则默认使用结构体名称
PRISM_CLASS_ATTRIBUTE(prism::sql::attributes::Attr_sql_class_alias, database_table, "table_database_table")
// sql 主键,2个以上主键生成组合键
PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_isPrimaryKey, &database_table::id, true)
PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_isPrimaryKey, &database_table::guid, true)
// sql 别名
PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_alias, &database_table::guid, "Uid")
// sql 忽略字段参与sql
PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_ignore, &database_table::myFloat, true)
// sql 重写默认字段类型
PRISM_FIELD_ATTRIBUTE(prism::sql::sqlite3::attributes::Attr_sql_field_datatype, &database_table::myDouble, "INT")

TEST_CASE("test prism")
{
    SECTION("test sqlite insert single table  ")
    {
        std::vector<std::shared_ptr<database_table>> models{
            std::make_shared<database_table>(),
            std::make_shared<database_table>(),
            std::make_shared<database_table>()
        };

        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::insert<database_table>(models);
        std::cout << "============================================="  << std::endl;
        std::cout << "insert sql"  << std::endl;
        std::cout << "============================================="  << std::endl;
        std::cout << sql << std::endl;

    }

    SECTION("test sqlite query single table  ")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::queryTable<database_table>();
        std::cout << "============================================="  << std::endl;
        std::cout << "query table"  << std::endl;
        std::cout << "============================================="  << std::endl;
        std::cout << sql << std::endl;

    }

    SECTION("test sqlite create table  ")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::createTable<database_table>();
        std::cout << "============================================="  << std::endl;
        std::cout << "create table"  << std::endl;
        std::cout << "============================================="  << std::endl;
        std::cout << sql << std::endl;

    }


    SECTION("reflect base type fields")
    {
        Derived d;

        std::cout << "====reflect base type's  fields  by static iterator  start: " << std::endl;
        prism::reflection::for_each_fields(d, [&](const char* fname, auto&& v) {
            std::cout << fname << " : " << v << std::endl;
        });

        std::cout << std::endl;
        std::cout << "====reflect base type's field  by name start " << std::endl;
        prism::reflection::field_do(d, "b1_int", [&](auto&& v) {
            std::cout << "b1_int"
                      << " : " << v << std::endl;
        });

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    SECTION("reflect all field")
    {
        tst_struct instance_1;
        tst_struct instance_2;

        instance_1.my_int = 1;
        instance_1.my_bool = true;
        instance_1.my_float = 1.1f;
        instance_1.my_double = 2.2;
        instance_1.my_string = "333";
        instance_1.my_longlong = 4444;
        instance_1.my_opt_str = std::make_optional<std::string>("hehe");
        instance_1.my_opt_int = 3;
        instance_1.my_ptr_int = new int(4);
        instance_1.my_opt_ptr = std::make_optional<bool*>();

        std::map<const char*, void*> map0;
        std::map<const char*, void*> map1;

        prism::reflection::field_do<prism::utilities::const_hash("tst")>(instance_1, "my_int", [](auto&& value) {
            std::cout << "tst _ field_do : ";
            append2ostream(std::cout, value);
        });

        // put instance_1 's field values into map0
        prism::reflection::for_each_fields(instance_1, [&](const char* fname, auto&& value) {
            map0[fname] = reinterpret_cast<void*>(&value);
        });

        // put 把map0's values into instance_2's fields
        prism::reflection::for_each_fields(instance_2, [&](const char* fname, auto&& value) {
            using t_ = std::decay_t<decltype(value)>;
            value = *reinterpret_cast<t_*>(map0[fname]);
        });
        // put instance_2 's field values into map1
        prism::reflection::for_each_fields(instance_1, [&](const char* fname, auto&& value) {
            map1[fname] = reinterpret_cast<void*>(&value);
        });

        prism::reflection::for_each_fields<prism::utilities::const_hash("tst")>(instance_2, [&](const char* fname, auto&& value) {
            using t_ = std::decay_t<decltype(value)>;
            t_ value1 = *reinterpret_cast<t_*>(map0[fname]);
            t_ value2 = *reinterpret_cast<t_*>(map1[fname]);
            std::cout << "instance_1." << fname << " : ";
            append2ostream(std::cout, value1);
            std::cout << "instance_2." << fname << " : ";
            append2ostream(std::cout, value2);
            std::cout << std::endl;
            // CHECK(value1 == value2);
        });
        delete instance_1.my_ptr_int;
    }

    SECTION("refelect fields ")
    {
        tst_struct inst;
        prism::reflection::field_do(inst, "my_string", [=](auto&& value) {
            if constexpr (std::is_same_v<std::string, std::decay_t<decltype(value)>>)
            {
                REQUIRE(inst.my_string == value);
            }
        });
    }

    SECTION("json测试")
    {
        tst_struct instance_1;
        instance_1.my_int = 1;
        instance_1.my_bool = true;
        instance_1.my_float = 1.12345678901234567890f;
        instance_1.my_double = 2.12345678901234567890;
        instance_1.my_string = "333";
        instance_1.my_longlong = 4444;
        instance_1.my_opt_str = std::make_optional<std::string>("hehe");
        instance_1.my_opt_int = 3;
        instance_1.my_ptr_int = new int(4);
        instance_1.my_opt_ptr = std::make_optional<bool *>(new bool(true));
        instance_1.my_timestamp = initTimestamp(2023, 6, 23, 12, 00, 00);
        instance_1.my_datetime = initDatetime(2023, 6, 23, 12, 12, 34);
        instance_1.my_map["hehe111"] = {instance_1};
        instance_1.my_shared_sub = std::make_shared<tst_sub_struct>();
        instance_1.my_shared_self = std::make_shared<tst_struct>(instance_1);
        instance_1.my_vec_sp = {{}, {}};
        instance_1.my_vec_sp[0].my_vec_sp = {{}, {}};
        instance_1.my_vec_sp[1].my_vec_sp = {{}, {}};
        instance_1.my_list_sp = {instance_1, instance_1};
        instance_1.my_map["hehe111"] = {};
        instance_1.my_map["hehe112"] = {};
        instance_1.my_map["hehe113"] = {};

        /**
         *  Because unordermap is unordered,
         *  it will cause json string comparisons to be unequal,
         *  so comment out these three lines of code,
         *   but it does not mean that unordermap is not supported
         **/
        // instance_1.my_unordermap["hehe114"] = {};
        // instance_1.my_unordermap["hehe114"] = {};
        // instance_1.my_unordermap["hehe115"] = {};

        std::string json;
        std::shared_ptr<tst_struct> t;
        // BENCHMARK("serialization  100 times")
        //{
        json = prism::json::toJsonString(std::move(instance_1), 2);
        std::cout << json << std::endl;

        //};

        // BENCHMARK("deserialization 100 times")
        //{
        t = prism::json::fromJsonString<tst_struct>(std::move(json));
        //};
        std::string json2 = prism::json::toJsonString(std::move(*t), 2);

        // std::cout << json2 << std::endl;
        // return;

        SECTION("struct.member")
        {
            CHECK(instance_1.my_bool == t->my_bool);
            CHECK(instance_1.my_datetime == t->my_datetime);
            CHECK(std::abs(instance_1.my_double - t->my_double) < 0.0001);
            CHECK(std::abs(instance_1.my_float - t->my_float) < 0.0001);
            CHECK(instance_1.my_int == t->my_int);
            CHECK(instance_1.my_longlong == t->my_longlong);
            CHECK(instance_1.my_opt_int == t->my_opt_int);
            CHECK(instance_1.my_opt_str == t->my_opt_str);

            if (instance_1.my_sptr_int != nullptr && t->my_sptr_int != nullptr)
                CHECK(*instance_1.my_sptr_int == *t->my_sptr_int);
            else
            {
                CHECK(instance_1.my_sptr_int == nullptr);
                CHECK(t->my_sptr_int == nullptr);
            }

            CHECK(instance_1.my_string == t->my_string);
            CHECK(instance_1.my_timestamp == t->my_timestamp);
            CHECK(instance_1.my_list_int.size() == t->my_list_int.size());
            CHECK(instance_1.my_list_int.front() == t->my_list_int.front());
            CHECK(instance_1.my_list_std_string.size() == t->my_list_std_string.size());
            CHECK(instance_1.my_list_std_string.front() == t->my_list_std_string.front());
            CHECK(instance_1.lang == t->lang);
        }

        SECTION("struct.struct.member")
        {
            CHECK(instance_1.my_struct.my_bool == t->my_struct.my_bool);
            CHECK(std::abs(instance_1.my_struct.my_double - t->my_struct.my_double) < 0.0001);
            CHECK(std::abs(instance_1.my_struct.my_float - t->my_struct.my_float) < 0.0001);
            CHECK(instance_1.my_struct.my_int == t->my_struct.my_int);
            CHECK(instance_1.my_struct.my_longlong == t->my_struct.my_longlong);
            CHECK(instance_1.my_struct.my_opt_int == t->my_struct.my_opt_int);
            CHECK(instance_1.my_struct.my_opt_ptr == t->my_struct.my_opt_ptr);
            CHECK(instance_1.my_struct.my_opt_str == t->my_struct.my_opt_str);
            CHECK(instance_1.my_struct.my_ptr_int == t->my_struct.my_ptr_int);
            CHECK(instance_1.my_struct.my_string == t->my_struct.my_string);
        }

        SECTION("struct.list<struct>.member")
        {
            CHECK(instance_1.my_list_sp.front().my_bool == t->my_list_sp.front().my_bool);
            CHECK(instance_1.my_list_sp.front().my_datetime == t->my_list_sp.front().my_datetime);
            CHECK(std::abs(instance_1.my_list_sp.front().my_double - t->my_list_sp.front().my_double) < 0.0001);
            CHECK(std::abs(instance_1.my_list_sp.front().my_float - t->my_list_sp.front().my_float) < 0.0001);
            CHECK(instance_1.my_list_sp.front().my_int == t->my_list_sp.front().my_int);
            CHECK(instance_1.my_list_sp.front().my_longlong == t->my_list_sp.front().my_longlong);
            CHECK(instance_1.my_list_sp.front().my_opt_int == t->my_list_sp.front().my_opt_int);

            if (instance_1.my_list_sp.front().my_opt_ptr.has_value() && t->my_list_sp.front().my_opt_ptr.has_value())
                CHECK(*instance_1.my_list_sp.front().my_opt_ptr.value() == *t->my_list_sp.front().my_opt_ptr.value());
            else
            {
                CHECK(instance_1.my_list_sp.front().my_opt_ptr.has_value() == false);
                CHECK(t->my_list_sp.front().my_opt_ptr.has_value() == false);
            }

            CHECK(instance_1.my_list_sp.front().my_opt_str == t->my_list_sp.front().my_opt_str);
            CHECK(*instance_1.my_list_sp.front().my_ptr_int == *t->my_list_sp.front().my_ptr_int);
            CHECK(*instance_1.my_list_sp.front().my_sptr_int == *t->my_list_sp.front().my_sptr_int);
            if (instance_1.my_list_sp.front().my_sptr_int != nullptr && t->my_list_sp.front().my_sptr_int != nullptr)
                CHECK(*instance_1.my_list_sp.front().my_sptr_int == *t->my_list_sp.front().my_sptr_int);
            else
            {
                CHECK(instance_1.my_list_sp.front().my_sptr_int == nullptr);
                CHECK(t->my_list_sp.front().my_sptr_int == nullptr);
            }
            CHECK(instance_1.my_list_sp.front().my_string == t->my_list_sp.front().my_string);
            CHECK(instance_1.my_list_sp.front().my_timestamp == t->my_list_sp.front().my_timestamp);
            CHECK(instance_1.my_list_sp.front().lang == t->my_list_sp.front().lang);
        }

        SECTION("struct.shared_ptr<struct>->member")
        {
            CHECK(instance_1.my_shared_sub->my_bool == t->my_shared_sub->my_bool);
            CHECK(instance_1.my_shared_sub->my_double == t->my_shared_sub->my_double);
            CHECK(instance_1.my_shared_sub->my_float == t->my_shared_sub->my_float);
            CHECK(instance_1.my_shared_sub->my_int == t->my_shared_sub->my_int);
            CHECK(instance_1.my_shared_sub->my_opt_int == t->my_shared_sub->my_opt_int);
            CHECK(instance_1.my_shared_sub->my_opt_ptr == t->my_shared_sub->my_opt_ptr);
            CHECK(instance_1.my_shared_sub->my_opt_str == t->my_shared_sub->my_opt_str);
            CHECK(instance_1.my_shared_sub->my_ptr_int == t->my_shared_sub->my_ptr_int);
            CHECK(instance_1.my_shared_sub->my_string == t->my_shared_sub->my_string);
            CHECK(instance_1.my_shared_sub->my_longlong == t->my_shared_sub->my_longlong);
        }
        // CHECK(json2 == json);
        std::ofstream ofs("json.json");
        std::ofstream ofs2("json2.json");
        ofs << json;
        ofs2 << json2;
    }
    SKIP();

    SECTION("test sqlite insert single table  ")
    {
        std::vector<std::shared_ptr<database_table>> models{
            std::make_shared<database_table>(), std::make_shared<database_table>(), std::make_shared<database_table>()};

        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::insert<database_table>(models);
        std::cout << "=============================================" << std::endl;
        std::cout << "insert sql" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << sql << std::endl;
    }

    SKIP();

    SECTION("test sqlite query single table  ")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::queryTable<database_table>();
        std::cout << "=============================================" << std::endl;
        std::cout << "query table" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << sql << std::endl;
    }

    SKIP();

    SECTION("test sqlite create table  ")
    {
        std::string sql = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>::createTable<database_table>();
        std::cout << "=============================================" << std::endl;
        std::cout << "create table" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << sql << std::endl;
    }

    SECTION("reflect base type fields")
    {
        Derived d;

        std::cout << "====reflect base type's  fields  by static iterator  start: " << std::endl;
        prism::reflection::for_each_fields(d, [&](const char *fname, auto &&v)
                                           { std::cout << fname << " : " << v << std::endl; });

        std::cout << std::endl;
        std::cout << "====reflect base type's field  by name start " << std::endl;
        prism::reflection::field_do(d, "b1_int",
                                    [&](auto &&v)
                                    {
                                        std::cout << "b1_int"
                                                  << " : " << v << std::endl;
                                    });

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }

    SECTION("reflect all field")
    {
        tst_struct instance_1;
        tst_struct instance_2;

        instance_1.my_int = 1;
        instance_1.my_bool = true;
        instance_1.my_float = 1.1f;
        instance_1.my_double = 2.2;
        instance_1.my_string = "333";
        instance_1.my_longlong = 4444;
        instance_1.my_opt_str = std::make_optional<std::string>("hehe");
        instance_1.my_opt_int = 3;
        instance_1.my_ptr_int = new int(4);
        instance_1.my_opt_ptr = std::make_optional<bool *>();

        std::map<const char *, void *> map0;
        std::map<const char *, void *> map1;

        prism::reflection::field_do<prism::utilities::const_hash("tst")>(instance_1, "my_int",
                                                                         [](auto &&value)
                                                                         {
                                                                             std::cout << "tst _ field_do : ";
                                                                             append2ostream(std::cout, value);
                                                                         });

        // put instance_1 's field values into map0
        prism::reflection::for_each_fields(instance_1, [&](const char *fname, auto &&value)
                                           { map0[fname] = reinterpret_cast<void *>(&value); });

        // put 把map0's values into instance_2's fields
        prism::reflection::for_each_fields(instance_2,
                                           [&](const char *fname, auto &&value)
                                           {
                                               using t_ = std::decay_t<decltype(value)>;
                                               value = *reinterpret_cast<t_ *>(map0[fname]);
                                           });
        // put instance_2 's field values into map1
        prism::reflection::for_each_fields(instance_1, [&](const char *fname, auto &&value)
                                           { map1[fname] = reinterpret_cast<void *>(&value); });

        prism::reflection::for_each_fields<prism::utilities::const_hash("tst")>(
            instance_2,
            [&](const char *fname, auto &&value)
            {
                using t_ = std::decay_t<decltype(value)>;
                t_ value1 = *reinterpret_cast<t_ *>(map0[fname]);
                t_ value2 = *reinterpret_cast<t_ *>(map1[fname]);
                std::cout << "instance_1." << fname << " : ";
                append2ostream(std::cout, value1);
                std::cout << "instance_2." << fname << " : ";
                append2ostream(std::cout, value2);
                std::cout << std::endl;
                // CHECK(value1 == value2);
            });
        delete instance_1.my_ptr_int;
    }

    SECTION("refelect fields ")
    {
        tst_struct inst;
        prism::reflection::field_do(inst, "my_string",
                                    [=](auto &&value)
                                    {
                                        if constexpr (std::is_same_v<std::string, std::decay_t<decltype(value)>>)
                                        {
                                            REQUIRE(inst.my_string == value);
                                        }
                                    });
    }
}
int main(int argc, const char **argv)
{
#ifdef _MSC_VER
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int result = Catch::Session().run(argc, argv);
    return result;
}
