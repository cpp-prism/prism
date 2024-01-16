#ifndef PRISM_PRISMSQL_HPP
#define PRISM_PRISMSQL_HPP

#include "prism.hpp"
#include <string>
#include <set>



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

struct Attr_sql_field_isCombinIndex{
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
    static inline std::string queryTable(const char* where = nullptr)
    {
        std::string sql = "SELECT ";
        bool isfirst = true;
        prism::reflection::for_each_fields(*(T*)nullptr,[&](const char* fname, [[maybe_unused]]auto&&value){
            std::optional<bool> ignore = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_ignore>(fname);
            if(ignore.has_value() && ignore)
                return;

            if(!isfirst)
                sql += ", ";
            else
                isfirst = false;

            std::optional<const char* > alias = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias.has_value())
                sql += alias.value();
            else
                sql += fname;

        });

        sql+= " FROM " ;

        std::optional<const char*> tableName = prism::attributes::getClassAttr<T,sql::attributes::Attr_sql_class_alias>();
        if(tableName.has_value())
            sql += tableName.value();
        else
            sql += prism::utilities::typeName<T>::value;

        if(where)
        {
            sql += " ";
            sql += where;
        }

        sql += ";";

        return sql;
    }

    template<class T>
    static inline std::string createTable()
    {
        std::string sql = "CREATE TABLE IF NOT EXISTS ";
        std::optional<const char*> tableName = prism::attributes::getClassAttr<T,sql::attributes::Attr_sql_class_alias>();
        if(tableName.has_value())
            sql += tableName.value();
        else
            sql += prism::utilities::typeName<T>::value;

        sql += " (\n";

        std::vector<std::string> primaryKeys;
        prism::reflection::for_each_fields(*((T*)nullptr),[&](const char* fname, [[maybe_unused]]auto&& value){

            std::string alias = fname;
            std::optional<const char* > alias_opt = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias_opt.has_value())
                alias = alias_opt.value();

            std::optional<bool> primaryKey = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_isPrimaryKey>(fname);
            if(primaryKey.has_value() && primaryKey)
                primaryKeys.push_back(alias);
        });

        bool isfirst = true;
        prism::reflection::for_each_fields(*((T*)nullptr),[&](const char* fname, [[maybe_unused]]auto&& value){


            std::optional<bool> ignore = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_ignore>(fname);
            if(ignore.has_value() && ignore)
                return;

            if(isfirst)
                isfirst = false;

            std::optional<const char* > alias = prism::attributes::getFieldAttr<T,sql::attributes::Attr_sql_field_alias>(fname);
            if(alias.has_value())
                sql += alias.value();
            else
                sql += fname;
            sql += " ";


            std::optional<const char* > datatype = prism::attributes::getFieldAttr<T,sql::sqlite3::attributes::Attr_sql_field_datatype>(fname);
            if(datatype.has_value())
            {
                sql += datatype.value();
            }

            if(!isfirst)
                sql += ", \n";

        });

        sql += "PRIMARY KEY(";
        isfirst = true;
        for(std::string fn: primaryKeys)
        {
            if(isfirst)
                isfirst = false;
            else
                sql += ", ";

            sql += fn;

        }
        sql += ")\n);";

       return sql;
    }
};
} //namespace sqlite3




} //namespace sql
} //namespace prism

PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , int           , "INTEGER")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , long long     , "INTEGER")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , double        , "REAL")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , float         , "REAL")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , std::string   , "TEXT")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , const char*   , "Text")
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(::prism::sql::sqlite3::attributes::Attr_sql_field_datatype , bool          , "INTEGER") //0 or 1

#endif // PRISM_PRISMSQL_HPP
