# test models

```c++
#include <prism/prism.hpp>
#include <iostream>
#include <optional>

namespace mynamespace{
enum Enum_color{
  Undefine = 0,
  Black = 1,
  Red = 2
};

struct BaseModel
{
   std::string base_mystr = "guid";
   int base_myint = 1;
};
  
struct TstSubModel: public BaseModel
{
  Enum_color color;
};
  
struct TstModel: public BaseModel
{
  int myint = 0;
  double mydouble = 1.123;
  std::shared_ptr<TstSubModel> mysubmodel = std::make_shared<TstSubModel>();
};
  
struct Attr_alias{
  using value_type = const char*;
};
  
} // namespace mynamespace
```



# registe enum

```c++
PRISM_ENUM(mynamespace::Enum_color , { {mynamespace::black,"Black"},
                                       {mynamespace::red,"Red"} })
```



# registe fields

```c++
PRISM_FIELDS(mynamespace::BaseModel,base_myint,base_mystr)
PRISM_FIELDS(mynamespace::TstSubModel,color)
PRISM_FIELDS(mynamespace::TstModel,myint,mydouble,mysubmodel)
```



# registe  base types

```c++
PRISM_CLASS_BASE_TYPES(mynamespace::TstModel,mynamespace::BaseModel/* , others if has multiple...*/)
PRISM_CLASS_BASE_TYPES(mynamespace::TstSubModel,mynamespace::BaseModel)
```



# foreach fields

```c++
mynamespace::TstModel model{};
prism::reflection::for_each_fields(model,[](const char* fieldName,auto&& value){
  // do get or set value 
});
```



# reflect a field by field's name

```c++
TstModel model{};
prism::reflection::field_do(model,"myint"[](auto&& value){
  // do get or set value
});
```



# ignore a field 

```
PRISM_IGNORE_FIELD(mynamespace::TstModel , myint, "someBusinessName")
PRISM_IGNORE_FIELD(mynamespace::TstModel , mydoule, "someBusinessName")

int main(){


    mynamespace::TstModel model{};

    int count = 0;
    prism::reflection::for_each_fields(model,[&](const char* fname,auto&& value){
        std::cout << fname << std::endl;
        ++count;
    }); //print all field's names of model

    std::cout << "count : " << count << std::endl; //print 5


    count = 0;
    prism::reflection::for_each_fields<prism::utilities::const_hash("someBusinessName")>(model,[](const char* fname,auto&& value){
        std::cout << fname << std::endl;
        ++count;
    }); //will not print "myint" and "mydouble"
    std::cout << "count : " << count << std::endl; //print 3 


}
```



# field's atrribute

```c++
PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(mynamespace::Attr_alias, int,"int type field's default alias")
PRISM_FIELD_ATTRIBUTE(mynamespace::Attr_alias, &mynamespace::TstModel::myint,"TestModel::myint alias")

int main(){
  
  std::optional<const char*> alias ;
  
  alias = PRISM_GET_FIELD_ATTRIBUTE(&mynamespace::BaseModel::base_myint, mynamespace::Attr_alias);
  if(alias.has_value())
    std::cout << alias.value() << std::endl; // will print "int type field's default alias"
  
  alias = PRISM_GET_FIELD_ATTRIBUTE(&mynamespace::TstModel::myint, mynamespace::Attr_alias);
  if(alias.has_value())
    std::cout << alias.value() << std::endl; // will print "TestModel::myint alias"
  
}
  


```



# class's attribute

```c++
PRISM_CLASS_ATTRIBUTE(mynamespace::TstModel, mynamespace::Attr_alias,"class alias")

int main()
{
   std::optional<const char*> alias = PRISM_GET_CLASS_ATTRIBUTE(mynamespace::TstModel,mynamespace::Attr_alias);
   if(alias.has_value())
     std::cout << alias.value() << std::endl; // will print "class alias"
}
```



# prism json

```c++
#include <prism/prismJson.hpp>

PRISM_IGNORE_JSON_FIELD(mynamespace::TstModel,myint)

PRISM_FIELD_ATTRIBUTE(prism::json::attributes::Attr_json_alias,&mynamespace::TestModel::my_int,
                      "MY_INT_ALIAS")


int main()
{
  std::shared_ptr<mynamespace::TstModel> model = std::make_shared<mynamespace::TstModel>();
  std::json json = prism::json::toJsonString(*model,4);//  Indentation with 4 spaces.default is 0 spaces.
  std::shared_ptr<mynamespace::TstModel> model2 = prism::json::fromJsonString<mynamespace::TstModel>(json);
}

```



# prism sql (just generate sql string)

```c++
#include <prism/prismSql.hpp>
struct database_table
{
    int id = 1;
    std::string  guid = "2";
    bool  myBool = true;
    float myFloat = 4.1;
    double myDouble = 5.230;
};

PRISM_FIELDS(database_table,id,guid,myBool,myFloat,myDouble);


PRISM_CLASS_ATTRIBUTE(prism::sql::attributes::Attr_sql_class_alias, database_table , "database_table_alias")

PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_isPrimaryKey, &database_table::id , true)
PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_isPrimaryKey , &database_table::guid , true)

PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_alias , &database_table::guid , "Uid")

PRISM_FIELD_ATTRIBUTE(prism::sql::attributes::Attr_sql_field_ignore , &database_table::myFloat , true)

PRISM_FIELD_ATTRIBUTE(prism::sql::sqlite3::attributes::Attr_sql_field_datatype , &database_table::myDouble , "INT")
  
int main()
{
    using prismSqlite3 = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>;
    //SECTION("test sqlite insert single table  ")
    {
        std::vector<std::shared_ptr<database_table>> models{
            std::make_shared<database_table>(),
            std::make_shared<database_table>(),
            std::make_shared<database_table>()
        };

        std::string sql = prismSqlite3::insert<database_table>(models);
        std::cout << "============================================="  << std::endl;
        std::cout << "insert sql"  << std::endl;
        std::cout << "============================================="  << std::endl;
        std::cout << sql << std::endl;

    }



    //SECTION("test sqlite query single table  ")
    {
        std::string sql = prismSqlite3::queryTable<database_table>();
        std::cout << "============================================="  << std::endl;
        std::cout << "query table"  << std::endl;
        std::cout << "============================================="  << std::endl;
        std::cout << sql << std::endl;

    }



    //SECTION("test sqlite create table  ")
    {
        std::string sql = prismSqlite3::createTable<database_table>();
        std::cout << "============================================="  << std::endl;
        std::cout << "create table"  << std::endl;
        std::cout << "============================================="  << std::endl;
        std::cout << sql << std::endl;

    }
}

```

