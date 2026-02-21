# Prism - C++ Reflection Framework

A lightweight C++17 header-only reflection framework with JSON/YAML/SQL support.

```c++
// Include all at once
#include <prism/prism.hpp>
#include <prism/prismJson.hpp>
#include <prism/prismYaml.hpp>
#include <prism/prismSql.hpp>
```

## Features

- **Runtime reflection** - Iterate fields, get/set values by name
- **JSON serialization** - Serialize/deserialize objects to/from JSON
- **YAML serialization** - Serialize/deserialize objects to/from YAML
- **SQL generation** - Generate SQL statements from C++ structures
- **Attribute support** - Custom annotations for fields and classes

## Quick Start

```c++
#include <prism/prism.hpp>
#include <prism/prismJson.hpp>
#include <iostream>

// 1. Define your structure
struct Person {
    std::string name;
    int age;
    std::vector<std::string> hobbies;
};
PRISM_FIELDS(Person, name, age, hobbies);

// 2. JSON serialization
int main() {
    Person p{"Alice", 30, {"reading", "coding"}};

    // Serialize to JSON
    std::string json = prism::json::toJsonString(p);
    std::cout << json << std::endl;
    // Output: {"name":"Alice","age":30,"hobbies":["reading","coding"]}

    // Deserialize from JSON
    auto p2 = prism::json::fromJsonString<Person>(json);
    std::cout << p2->name << " is " << p2->age << " years old" << std::endl;
}
```

## Extended Container Support

```c++
#include <set>
#include <unordered_set>
#include <deque>
#include <prism/prismJson.hpp>

// All standard containers work out of the box
std::set<int> numbers = {1, 2, 3};
std::string json = prism::json::toJsonString(numbers);
// Output: [1,2,3]

// Nested containers are fully supported
std::vector<std::set<int>> nested = {{1, 2}, {3, 4}};
std::string json2 = prism::json::toJsonString(nested);
// Output: [[1,2],[3,4]]
auto result = prism::json::fromJsonString<decltype(nested)>(json2);
```

## Enum Support

```c++
#include <prism/prism.hpp>

enum class Color { Red, Green, Blue };
PRISM_ENUM(Color, {{Color::Red, "Red"}, {Color::Green, "Green"}, {Color::Blue, "Blue"}})

Color c = Color::Red;
std::string json = prism::json::toJsonString(c);
// Output: "Red"
```

## Field Attributes

```c++
#include <prism/prism.hpp>
#include <optional>

struct Config {
    std::string database_url;
    int timeout = 30;
};
PRISM_FIELDS(Config, database_url, timeout);

// Must be in global scope, not inside class
PRISM_IGNORE_JSON_FIELD(Config, database_url)

// Use alias in JSON
PRISM_FIELD_ATTRIBUTE(prism::json::attributes::Attr_json_alias,
                      &Config::timeout, "connect_timeout")

// Get field attribute
int main() {
    std::optional<const char*> alias = prism::attributes::getFieldAttr<
        Config, prism::json::attributes::Attr_json_alias>(&Config::timeout);
    if (alias.has_value()) {
        std::cout << *alias << std::endl; // print "connect_timeout"
    }
}
```

## SQL Generation

```c++
#include <prism/prismSql.hpp>

struct User {
    int id;
    std::string email;
};
PRISM_FIELDS(User, id, email);

PRISM_CLASS_ATTRIBUTE(prism::sql::attributes::Attr_sql_class_alias,
                      User, "users")

// Generate SQL statements
using Sqlite3 = prism::sql::Sql<prism::sql::sqlite3::Sqlite3>;

User user{1, "alice@example.com"};
std::string insert = Sqlite3::insert<User>(&user);
std::string query = Sqlite3::queryTable<User>();
std::string create = Sqlite3::createTable<User>();
```

## API Reference

### Reflection
- `prism::reflection::for_each_fields(obj, lambda)` - Iterate all fields
- `prism::reflection::field_do(obj, name, lambda)` - Access field by name

### JSON
- `prism::json::toJsonString(obj, indent=0)` - Serialize to JSON
- `prism::json::fromJsonString<T>(json)` - Deserialize from JSON

### YAML

```c++
#include <prism/prismYaml.hpp>

Person p{"Bob", 25, {"music"}};

// Serialize to YAML (block format)
std::string yaml = prism::yaml::toYamlString(p);

// Deserialize from YAML
auto p2 = prism::yaml::fromYamlString<Person>(yaml);
```

## Tests

Run tests with:
```bash
cd build && make test
```
