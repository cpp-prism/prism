#ifndef PRISM_PRISMSQL_HPP
#define PRISM_PRISMSQL_HPP

#include "prism.hpp"
#include <string>
#include <set>
#include <sstream>



namespace prism
{
namespace sql
{

namespace attributes
{

struct Attr_sql_class_alias{
    using value_type= const char*;
};


struct Attr_sql_field_ignore{
    using value_type= bool;
};

struct Attr_sql_field_isPrimaryKey{
    using value_type= bool;
};



struct Attr_sql_field_alias{
    using value_type= const char*;
};

} //namespace attributes



namespace privates
{



} //namespace privates




template<class derived>
struct Sql
{
    template<class T>
    static inline std::string createTable()
    {
        return derived::template createTable<T>();
    }

    template<class T>
    static inline std::string queryTable(const char* where = nullptr)
    {
        return derived::template queryTable<T>(where);
    }

    template<class T>
    static inline std::string insert(std::vector<std::shared_ptr<T>> models)
    {
        return derived::template insert(models);
    }
};

namespace sqlite3
{
namespace attributes
{
struct Attr_sql_field_datatype{
    using value_type= const char*;
};

} //namespace attributes
struct Sqlite3:public Sql<Sqlite3>
{
    template<class T>
    static inline std::string insert(std::vector<std::shared_ptr<T>> models)
    {
        std::stringstream sql;
        sql << "INSERT INTO ";
        std::optional<const char*> tableName = prism::attributes::getClassAttr<T,sql::attributes::Attr_sql_class_alias>();
        if(tableName.has_value())
            sql << tableName.value();
        else
            sql << prism::utilities::typeName<T>::value;

        sql << "\n(";

        bool isfirst = true;
#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"
#endif
        prism::reflection::for_each_fields(*(T*)nullptr,[&](const char* fname, [[maybe_unused]]auto&&value){
#if __clang__
#pragma clang diagnostic pop
#endif
            std::optional<bool> ignore = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_ignore>(fname);
            if(ignore.has_value() && ignore)
                return;

            if(!isfirst)
                sql << ", ";
            else
                isfirst = false;

            std::optional<const char* > alias = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias.has_value())
                sql << alias.value();
            else
                sql << fname;

        });

        sql << ") VALUES \n" ;

        bool isModelfirst = true;
        for(std::shared_ptr<T> item : models)
        {
            if(!isModelfirst)
                sql << ",\n";
            else
                isModelfirst = false;
            sql << "(" ;

            isfirst = true;
            prism::reflection::for_each_fields(*item,[&](const char* fname, [[maybe_unused]]auto&&value){
                std::optional<bool> ignore = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_ignore>(fname);
                if(ignore.has_value() && ignore)
                    return;

                if(!isfirst)
                    sql << ", ";
                else
                    isfirst = false;


                std::optional<const char* > datatype = prism::attributes::getFieldAttr<T,sql::sqlite3::attributes::Attr_sql_field_datatype>(fname);
                if(!datatype.has_value())
                {
                    std::string msg = std::string("field:") + fname +" hadn't config sql datatype";
                    std::cout << msg << std::endl;
                }

                using v_t = std::decay_t<std::decay_t<decltype(value)>>;
                if (std::is_same_v<bool,v_t>)
                {
                    sql << std::boolalpha << value;
                }
                else if(!std::strcmp(datatype.value() , "TEXT"))
                {
                    sql << "\"" << value << "\"";
                }
                else
                    sql << value;


            });
            sql << ")" ;
        }

        sql << ";";

        return sql.str();
    }

    template<class T>
    static inline std::string queryTable(const char* where = nullptr)
    {
        std::stringstream sql;
        sql << "SELECT ";
        bool isfirst = true;
#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"
#endif
        prism::reflection::for_each_fields(*(T*)nullptr,[&](const char* fname, [[maybe_unused]]auto&&value){
#if __clang__
#pragma clang diagnostic pop
#endif
            std::optional<bool> ignore = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_ignore>(fname);
            if(ignore.has_value() && ignore)
                return;

            if(!isfirst)
                sql << ", ";
            else
                isfirst = false;

            std::optional<const char* > alias = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias.has_value())
                sql << alias.value();
            else
                sql << fname;

        });

        sql << " FROM " ;

        std::optional<const char*> tableName = prism::attributes::getClassAttr<T,sql::attributes::Attr_sql_class_alias>();
        if(tableName.has_value())
            sql << tableName.value();
        else
            sql << prism::utilities::typeName<T>::value;

        if(where)
        {
            sql << " ";
            sql << where;
        }

        sql << ";";

        return sql.str();
    }

    template<class T>
    static inline std::string createTable()
    {
        std::stringstream sql;
        sql << "CREATE TABLE IF NOT EXISTS ";
        std::optional<const char*> tableName = prism::attributes::getClassAttr<T,sql::attributes::Attr_sql_class_alias>();
        if(tableName.has_value())
            sql << tableName.value();
        else
            sql << prism::utilities::typeName<T>::value;

        sql << " (\n";

        std::vector<std::string> primaryKeys;
#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"
#endif
        prism::reflection::for_each_fields(*(T*)nullptr,[&](const char* fname, [[maybe_unused]]auto&& value){
#if __clang__
#pragma clang diagnostic pop
#endif

            std::string alias = fname;
            std::optional<const char* > alias_opt = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias_opt.has_value())
                alias = alias_opt.value();

            std::optional<bool> primaryKey = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_isPrimaryKey>(fname);
            if(primaryKey.has_value() && primaryKey)
                primaryKeys.push_back(alias);
        });

        bool isfirst = true;
#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnull-dereference"
#endif
        prism::reflection::for_each_fields(*(T*)nullptr,[&](const char* fname, [[maybe_unused]]auto&& value){
#if __clang__
#pragma clang diagnostic pop
#endif


            std::optional<bool> ignore = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_ignore>(fname);
            if(ignore.has_value() && ignore)
                return;

            if(isfirst)
                isfirst = false;
            else
                sql << ", \n";


            std::optional<const char* > alias = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias.has_value())
                sql << alias.value();
            else
                sql << fname;
            sql << " ";


            std::optional<const char* > datatype = prism::attributes::getFieldAttr<T,sql::sqlite3::attributes::Attr_sql_field_datatype>(fname);
            if(datatype.has_value())
            {
                sql << datatype.value();
            }


        });

        if(primaryKeys.size())
        {
            sql << ",\nPRIMARY KEY(";
            isfirst = true;
            for(std::string fn: primaryKeys)
            {
                if(isfirst)
                    isfirst = false;
                else
                    sql << ", ";

                sql << fn;

            }
            sql << ")\n";
        }
        sql << ");";

       return sql.str();
    }
};
} //namespace sqlite3




} //namespace sql
} //namespace prism

PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , int           , "INTEGER")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , long          , "INTEGER")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , long long     , "INTEGER")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , double        , "REAL")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , float         , "REAL")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , std::string   , "TEXT")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , const char*   , "TEXT")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , bool          , "INTEGER") //0 or 1

#endif // PRISM_PRISMSQL_HPP
