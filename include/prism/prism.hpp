#ifndef PRISM_PRISM_H
#define PRISM_PRISM_H

#include "utilities/typeName.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#define PP_NARG(...) PP_NARG_(__VA_ARGS__, PP_RSEQ_N())
#define PP_NARG_(...) PP_ARG_N(__VA_ARGS__)
#define PP_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22,  \
                 _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42,   \
                 _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,   \
                 _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82,   \
                 _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101,      \
                 _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, \
                 _119, _120, _121, _122, _123, _124, _125, N, ...)                                                     \
    N
#define PP_RSEQ_N()                                                                                                    \
    125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, \
        102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, \
        75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,    \
        48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22,    \
        21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// ENUM

#define PRISM_ENUM_INITLIST_EXPAND(n, Enum, ...) __VA_ARGS__;
#define PRISM_ENUM(en_t, ...)                                                                    \
    namespace prism                                                                              \
    {                                                                                            \
    namespace enums                                                                              \
    {                                                                                            \
    template <class T>                                                                           \
    struct enum_info<T, std::enable_if_t<std::is_same_v<T, en_t>, void>>                         \
    {                                                                                            \
        static std::map<T, const char*>& getEnumMap()                                            \
        {                                                                                        \
            static std::map<T, const char*> map_                                                 \
                PRISM_ENUM_INITLIST_EXPAND(PP_NARG(__VA_ARGS__), en_t, __VA_ARGS__) return map_; \
        }                                                                                        \
        static std::map<const char*, T, prism::enums::compareCharPointers>& getStrMap()          \
        {                                                                                        \
            static std::map<const char*, T, prism::enums::compareCharPointers> map_;             \
            if (map_.size() == 0)                                                                \
            {                                                                                    \
                for (const std::pair<T, const char*> item : getEnumMap())                        \
                {                                                                                \
                    map_[item.second] = item.first;                                              \
                }                                                                                \
            }                                                                                    \
            return map_;                                                                         \
        }                                                                                        \
        constexpr static const char* tostring(const T& e)                                        \
        {                                                                                        \
            return getEnumMap()[e];                                                              \
        }                                                                                        \
        constexpr static T fromstring(const char* str)                                           \
        {                                                                                        \
            auto& sm = getStrMap();                                                              \
            return sm[str];                                                                      \
        }                                                                                        \
        static std::vector<std::string>& getStrVector()                                          \
        {                                                                                        \
            static std::vector<std::string> vector_;                                             \
            if (vector_.size() == 0)                                                             \
            {                                                                                    \
                for (const std::pair<T, const char*> item : getEnumMap())                        \
                {                                                                                \
                    vector_.push_back(item.second);                                              \
                }                                                                                \
            }                                                                                    \
            return vector_;                                                                      \
        }                                                                                        \
    };                                                                                           \
    }                                                                                            \
    }

// ATTRIBUTE


#define PRISM_FIELDTYPE_DEFAULT_ATTRIBUTE(AttributeName, FieldType, DefalutValue) \
    namespace prism                                                               \
    {                                                                             \
    namespace attributes                                                          \
    {                                                                             \
    namespace privates                                                            \
    {                                                                             \
    template <class T, FieldType T::*fn>                                          \
    struct field_attribute<T, FieldType, fn, AttributeName>                       \
    {                                                                             \
        constexpr static std::stdoptional<AttributeName::value_type> value()      \
        {                                                                         \
            return DefalutValue;                                                  \
        }                                                                         \
    };                                                                            \
    }                                                                             \
    }                                                                             \
    }

#define PRISM_FIELD_ATTRIBUTE(AttributeName, memberFucPtr, DefalutValue)                              \
    namespace prism                                                                                   \
    {                                                                                                 \
    namespace attributes                                                                              \
    {                                                                                                 \
    namespace privates                                                                                \
    {                                                                                                 \
    template <>                                                                                       \
    struct field_attribute<decltype(prism::utilities::getT(std::declval<decltype(memberFucPtr)>())),  \
                           decltype(prism::utilities::getMT(std::declval<decltype(memberFucPtr)>())), \
                           memberFucPtr,                                                              \
                           AttributeName>                                                             \
    {                                                                                                 \
        constexpr static std::stdoptional<AttributeName::value_type> value()                          \
        {                                                                                             \
            return DefalutValue;                                                                      \
        }                                                                                             \
    };                                                                                                \
    }                                                                                                 \
    }                                                                                                 \
    }

#define PRISM_GET_FIELD_ATTRIBUTE(memberFucPtr, AttributeName)                                                                \
    ::prism::attributes::privates::field_attribute<decltype(prism::utilities::getT(std::declval<decltype(memberFucPtr)>())),  \
                                                   decltype(prism::utilities::getMT(std::declval<decltype(memberFucPtr)>())), \
                                                   memberFucPtr,                                                              \
                                                   AttributeName>::value();


#define PRISM_CLASS_ATTRIBUTE(AttributeName, Class, DefalutValue)                                     \
    namespace prism                                                                                   \
    {                                                                                                 \
    namespace attributes                                                                              \
    {                                                                                                 \
    namespace privates                                                                                \
    {                                                                                                 \
    template <>                                                                                       \
    struct class_attribute<Class,AttributeName>                                                       \
    {                                                                                                 \
        constexpr static std::stdoptional<AttributeName::value_type> value()                          \
        {                                                                                             \
            return DefalutValue;                                                                      \
        }                                                                                             \
    };                                                                                                \
    }                                                                                                 \
    }                                                                                                 \
    }

#define PRISM_GET_CLASS_ATTRIBUTE(Class, AttributeName) ::prism::attributes::privates::class_attribute<Class,AttributeName>::value();





#define PRISM_IGNORE_FIELD(Class, Field, Business)                                                                                                           \
    namespace prism                                                                                                                                          \
    {                                                                                                                                                        \
    namespace attributes                                                                                                                                     \
    {                                                                                                                                                        \
    namespace privates                                                                                                                                       \
    {                                                                                                                                                        \
    template <>                                                                                                                                              \
    constexpr inline bool is_field_ignore<Class, decltype(prism::utilities::getMT(&Class::Field)), &Class::Field, prism::utilities::const_hash(#Business)>() \
    {                                                                                                                                                        \
        return true;                                                                                                                                         \
    }                                                                                                                                                        \
    }                                                                                                                                                        \
    }                                                                                                                                                        \
    }

#define AT_LEFT130(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT129(Class, __VA_ARGS__)
#define AT_LEFT129(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT128(Class, __VA_ARGS__)
#define AT_LEFT128(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT127(Class, __VA_ARGS__)
#define AT_LEFT127(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT126(Class, __VA_ARGS__)
#define AT_LEFT126(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT125(Class, __VA_ARGS__)
#define AT_LEFT125(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT124(Class, __VA_ARGS__)
#define AT_LEFT124(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT123(Class, __VA_ARGS__)
#define AT_LEFT123(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT122(Class, __VA_ARGS__)
#define AT_LEFT122(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT121(Class, __VA_ARGS__)
#define AT_LEFT121(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT120(Class, __VA_ARGS__)
#define AT_LEFT120(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT119(Class, __VA_ARGS__)
#define AT_LEFT119(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT118(Class, __VA_ARGS__)
#define AT_LEFT118(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT117(Class, __VA_ARGS__)
#define AT_LEFT117(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT116(Class, __VA_ARGS__)
#define AT_LEFT116(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT115(Class, __VA_ARGS__)
#define AT_LEFT115(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT114(Class, __VA_ARGS__)
#define AT_LEFT114(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT113(Class, __VA_ARGS__)
#define AT_LEFT113(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT112(Class, __VA_ARGS__)
#define AT_LEFT112(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT111(Class, __VA_ARGS__)
#define AT_LEFT111(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT110(Class, __VA_ARGS__)
#define AT_LEFT110(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT109(Class, __VA_ARGS__)
#define AT_LEFT109(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT108(Class, __VA_ARGS__)
#define AT_LEFT108(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT107(Class, __VA_ARGS__)
#define AT_LEFT107(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT106(Class, __VA_ARGS__)
#define AT_LEFT106(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT105(Class, __VA_ARGS__)
#define AT_LEFT105(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT104(Class, __VA_ARGS__)
#define AT_LEFT104(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT103(Class, __VA_ARGS__)
#define AT_LEFT103(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT102(Class, __VA_ARGS__)
#define AT_LEFT102(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT101(Class, __VA_ARGS__)
#define AT_LEFT101(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT100(Class, __VA_ARGS__)
#define AT_LEFT100(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT99(Class, __VA_ARGS__)
#define AT_LEFT99(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT98(Class, __VA_ARGS__)
#define AT_LEFT98(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT97(Class, __VA_ARGS__)
#define AT_LEFT97(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT96(Class, __VA_ARGS__)
#define AT_LEFT96(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT95(Class, __VA_ARGS__)
#define AT_LEFT95(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT94(Class, __VA_ARGS__)
#define AT_LEFT94(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT93(Class, __VA_ARGS__)
#define AT_LEFT93(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT92(Class, __VA_ARGS__)
#define AT_LEFT92(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT91(Class, __VA_ARGS__)
#define AT_LEFT91(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT90(Class, __VA_ARGS__)
#define AT_LEFT90(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT89(Class, __VA_ARGS__)
#define AT_LEFT89(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT88(Class, __VA_ARGS__)
#define AT_LEFT88(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT87(Class, __VA_ARGS__)
#define AT_LEFT87(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT86(Class, __VA_ARGS__)
#define AT_LEFT86(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT85(Class, __VA_ARGS__)
#define AT_LEFT85(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT84(Class, __VA_ARGS__)
#define AT_LEFT84(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT83(Class, __VA_ARGS__)
#define AT_LEFT83(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT82(Class, __VA_ARGS__)
#define AT_LEFT82(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT81(Class, __VA_ARGS__)
#define AT_LEFT81(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT80(Class, __VA_ARGS__)
#define AT_LEFT80(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT79(Class, __VA_ARGS__)
#define AT_LEFT79(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT78(Class, __VA_ARGS__)
#define AT_LEFT78(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT77(Class, __VA_ARGS__)
#define AT_LEFT77(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT76(Class, __VA_ARGS__)
#define AT_LEFT76(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT75(Class, __VA_ARGS__)
#define AT_LEFT75(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT74(Class, __VA_ARGS__)
#define AT_LEFT74(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT73(Class, __VA_ARGS__)
#define AT_LEFT73(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT72(Class, __VA_ARGS__)
#define AT_LEFT72(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT71(Class, __VA_ARGS__)
#define AT_LEFT71(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT70(Class, __VA_ARGS__)
#define AT_LEFT70(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT69(Class, __VA_ARGS__)
#define AT_LEFT69(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT68(Class, __VA_ARGS__)
#define AT_LEFT68(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT67(Class, __VA_ARGS__)
#define AT_LEFT67(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT66(Class, __VA_ARGS__)
#define AT_LEFT66(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT65(Class, __VA_ARGS__)
#define AT_LEFT65(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT64(Class, __VA_ARGS__)
#define AT_LEFT64(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT63(Class, __VA_ARGS__)
#define AT_LEFT63(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT62(Class, __VA_ARGS__)
#define AT_LEFT62(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT61(Class, __VA_ARGS__)
#define AT_LEFT61(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT60(Class, __VA_ARGS__)
#define AT_LEFT60(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT59(Class, __VA_ARGS__)
#define AT_LEFT59(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT58(Class, __VA_ARGS__)
#define AT_LEFT58(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT57(Class, __VA_ARGS__)
#define AT_LEFT57(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT56(Class, __VA_ARGS__)
#define AT_LEFT56(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT55(Class, __VA_ARGS__)
#define AT_LEFT55(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT54(Class, __VA_ARGS__)
#define AT_LEFT54(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT53(Class, __VA_ARGS__)
#define AT_LEFT53(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT52(Class, __VA_ARGS__)
#define AT_LEFT52(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT51(Class, __VA_ARGS__)
#define AT_LEFT51(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT50(Class, __VA_ARGS__)
#define AT_LEFT50(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT49(Class, __VA_ARGS__)
#define AT_LEFT49(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT48(Class, __VA_ARGS__)
#define AT_LEFT48(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT47(Class, __VA_ARGS__)
#define AT_LEFT47(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT46(Class, __VA_ARGS__)
#define AT_LEFT46(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT45(Class, __VA_ARGS__)
#define AT_LEFT45(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT44(Class, __VA_ARGS__)
#define AT_LEFT44(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT43(Class, __VA_ARGS__)
#define AT_LEFT43(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT42(Class, __VA_ARGS__)
#define AT_LEFT42(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT41(Class, __VA_ARGS__)
#define AT_LEFT41(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT40(Class, __VA_ARGS__)
#define AT_LEFT40(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT39(Class, __VA_ARGS__)
#define AT_LEFT39(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT38(Class, __VA_ARGS__)
#define AT_LEFT38(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT37(Class, __VA_ARGS__)
#define AT_LEFT37(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT36(Class, __VA_ARGS__)
#define AT_LEFT36(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT35(Class, __VA_ARGS__)
#define AT_LEFT35(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT34(Class, __VA_ARGS__)
#define AT_LEFT34(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT33(Class, __VA_ARGS__)
#define AT_LEFT33(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT32(Class, __VA_ARGS__)
#define AT_LEFT32(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT31(Class, __VA_ARGS__)
#define AT_LEFT31(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT30(Class, __VA_ARGS__)
#define AT_LEFT30(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT29(Class, __VA_ARGS__)
#define AT_LEFT29(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT28(Class, __VA_ARGS__)
#define AT_LEFT28(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT27(Class, __VA_ARGS__)
#define AT_LEFT27(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT26(Class, __VA_ARGS__)
#define AT_LEFT26(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT25(Class, __VA_ARGS__)
#define AT_LEFT25(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT24(Class, __VA_ARGS__)
#define AT_LEFT24(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT23(Class, __VA_ARGS__)
#define AT_LEFT23(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT22(Class, __VA_ARGS__)
#define AT_LEFT22(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT21(Class, __VA_ARGS__)
#define AT_LEFT21(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT20(Class, __VA_ARGS__)
#define AT_LEFT20(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT19(Class, __VA_ARGS__)
#define AT_LEFT19(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT18(Class, __VA_ARGS__)
#define AT_LEFT18(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT17(Class, __VA_ARGS__)
#define AT_LEFT17(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT16(Class, __VA_ARGS__)
#define AT_LEFT16(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT15(Class, __VA_ARGS__)
#define AT_LEFT15(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT14(Class, __VA_ARGS__)
#define AT_LEFT14(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT13(Class, __VA_ARGS__)
#define AT_LEFT13(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT12(Class, __VA_ARGS__)
#define AT_LEFT12(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT11(Class, __VA_ARGS__)
#define AT_LEFT11(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT10(Class, __VA_ARGS__)
#define AT_LEFT10(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT9(Class, __VA_ARGS__)
#define AT_LEFT9(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT8(Class, __VA_ARGS__)
#define AT_LEFT8(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT7(Class, __VA_ARGS__)
#define AT_LEFT7(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT6(Class, __VA_ARGS__)
#define AT_LEFT6(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT5(Class, __VA_ARGS__)
#define AT_LEFT5(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT4(Class, __VA_ARGS__)
#define AT_LEFT4(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT3(Class, __VA_ARGS__)
#define AT_LEFT3(Class, f1, ...) AT_INSERT_ITM(Class, f1) AT_LEFT2(Class, __VA_ARGS__)
#define AT_LEFT2(Class, f1, ...) AT_INSERT_ITM(Class, f1)

#define AT_FIELD_L2R(Class, ...) AT_FIELD_L2R2(PP_NARG(Class, __VA_ARGS__), Class, __VA_ARGS__)
#define AT_FIELD_L2R2(n, Class, ...) AT_FIELD_LEFT(n, Class, __VA_ARGS__)
#define AT_FIELD_LEFT(n, Class, ...)          \
    AT_ONLY_FIRST_BEFORE(Class, __VA__ARGS__) \
    AT_LEFT##n(Class, __VA_ARGS__)            \
        AT_ONLY_FIRST_AFTER(Class, __VA_ARGS__)

#define AT_ONLY_FIRST_BEFORE(Class, ...)                           \
    namespace prism                                                \
    {                                                              \
    namespace attributes                                           \
    {                                                              \
    namespace privates                                             \
    {                                                              \
    template <class AT>                                            \
    struct st_field_attribute_do<Class, AT>                        \
    {                                                              \
        template <class LAM>                                       \
        constexpr static void run(const char* fname, LAM&& lambda) \
        {                                                          \
            switch (prism::utilities::const_hash(fname))           \
            {

#define AT_INSERT_ITM(Class, Field1)                                                                                                                 \
    case prism::utilities::const_hash(#Field1):                                                                                                      \
        lambda(prism::attributes::privates::field_attribute<Class, decltype(prism::utilities::getMT(&Class::Field1)), &Class::Field1, AT>::value()); \
        break;

#define AT_ONLY_FIRST_AFTER(Class, Field1, ...) \
    }                                           \
    }                                           \
    }                                           \
    ;                                           \
    }                                           \
    }                                           \
    }

// FIELD DO
#define FD_LEFT130(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT129(Class, __VA_ARGS__)
#define FD_LEFT129(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT128(Class, __VA_ARGS__)
#define FD_LEFT128(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT127(Class, __VA_ARGS__)
#define FD_LEFT127(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT126(Class, __VA_ARGS__)
#define FD_LEFT126(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT125(Class, __VA_ARGS__)
#define FD_LEFT125(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT124(Class, __VA_ARGS__)
#define FD_LEFT124(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT123(Class, __VA_ARGS__)
#define FD_LEFT123(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT122(Class, __VA_ARGS__)
#define FD_LEFT122(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT121(Class, __VA_ARGS__)
#define FD_LEFT121(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT120(Class, __VA_ARGS__)
#define FD_LEFT120(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT119(Class, __VA_ARGS__)
#define FD_LEFT119(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT118(Class, __VA_ARGS__)
#define FD_LEFT118(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT117(Class, __VA_ARGS__)
#define FD_LEFT117(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT116(Class, __VA_ARGS__)
#define FD_LEFT116(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT115(Class, __VA_ARGS__)
#define FD_LEFT115(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT114(Class, __VA_ARGS__)
#define FD_LEFT114(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT113(Class, __VA_ARGS__)
#define FD_LEFT113(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT112(Class, __VA_ARGS__)
#define FD_LEFT112(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT111(Class, __VA_ARGS__)
#define FD_LEFT111(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT110(Class, __VA_ARGS__)
#define FD_LEFT110(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT109(Class, __VA_ARGS__)
#define FD_LEFT109(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT108(Class, __VA_ARGS__)
#define FD_LEFT108(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT107(Class, __VA_ARGS__)
#define FD_LEFT107(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT106(Class, __VA_ARGS__)
#define FD_LEFT106(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT105(Class, __VA_ARGS__)
#define FD_LEFT105(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT104(Class, __VA_ARGS__)
#define FD_LEFT104(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT103(Class, __VA_ARGS__)
#define FD_LEFT103(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT102(Class, __VA_ARGS__)
#define FD_LEFT102(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT101(Class, __VA_ARGS__)
#define FD_LEFT101(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT100(Class, __VA_ARGS__)
#define FD_LEFT100(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT99(Class, __VA_ARGS__)
#define FD_LEFT99(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT98(Class, __VA_ARGS__)
#define FD_LEFT98(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT97(Class, __VA_ARGS__)
#define FD_LEFT97(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT96(Class, __VA_ARGS__)
#define FD_LEFT96(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT95(Class, __VA_ARGS__)
#define FD_LEFT95(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT94(Class, __VA_ARGS__)
#define FD_LEFT94(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT93(Class, __VA_ARGS__)
#define FD_LEFT93(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT92(Class, __VA_ARGS__)
#define FD_LEFT92(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT91(Class, __VA_ARGS__)
#define FD_LEFT91(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT90(Class, __VA_ARGS__)
#define FD_LEFT90(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT89(Class, __VA_ARGS__)
#define FD_LEFT89(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT88(Class, __VA_ARGS__)
#define FD_LEFT88(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT87(Class, __VA_ARGS__)
#define FD_LEFT87(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT86(Class, __VA_ARGS__)
#define FD_LEFT86(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT85(Class, __VA_ARGS__)
#define FD_LEFT85(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT84(Class, __VA_ARGS__)
#define FD_LEFT84(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT83(Class, __VA_ARGS__)
#define FD_LEFT83(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT82(Class, __VA_ARGS__)
#define FD_LEFT82(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT81(Class, __VA_ARGS__)
#define FD_LEFT81(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT80(Class, __VA_ARGS__)
#define FD_LEFT80(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT79(Class, __VA_ARGS__)
#define FD_LEFT79(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT78(Class, __VA_ARGS__)
#define FD_LEFT78(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT77(Class, __VA_ARGS__)
#define FD_LEFT77(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT76(Class, __VA_ARGS__)
#define FD_LEFT76(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT75(Class, __VA_ARGS__)
#define FD_LEFT75(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT74(Class, __VA_ARGS__)
#define FD_LEFT74(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT73(Class, __VA_ARGS__)
#define FD_LEFT73(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT72(Class, __VA_ARGS__)
#define FD_LEFT72(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT71(Class, __VA_ARGS__)
#define FD_LEFT71(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT70(Class, __VA_ARGS__)
#define FD_LEFT70(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT69(Class, __VA_ARGS__)
#define FD_LEFT69(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT68(Class, __VA_ARGS__)
#define FD_LEFT68(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT67(Class, __VA_ARGS__)
#define FD_LEFT67(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT66(Class, __VA_ARGS__)
#define FD_LEFT66(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT65(Class, __VA_ARGS__)
#define FD_LEFT65(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT64(Class, __VA_ARGS__)
#define FD_LEFT64(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT63(Class, __VA_ARGS__)
#define FD_LEFT63(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT62(Class, __VA_ARGS__)
#define FD_LEFT62(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT61(Class, __VA_ARGS__)
#define FD_LEFT61(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT60(Class, __VA_ARGS__)
#define FD_LEFT60(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT59(Class, __VA_ARGS__)
#define FD_LEFT59(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT58(Class, __VA_ARGS__)
#define FD_LEFT58(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT57(Class, __VA_ARGS__)
#define FD_LEFT57(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT56(Class, __VA_ARGS__)
#define FD_LEFT56(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT55(Class, __VA_ARGS__)
#define FD_LEFT55(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT54(Class, __VA_ARGS__)
#define FD_LEFT54(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT53(Class, __VA_ARGS__)
#define FD_LEFT53(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT52(Class, __VA_ARGS__)
#define FD_LEFT52(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT51(Class, __VA_ARGS__)
#define FD_LEFT51(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT50(Class, __VA_ARGS__)
#define FD_LEFT50(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT49(Class, __VA_ARGS__)
#define FD_LEFT49(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT48(Class, __VA_ARGS__)
#define FD_LEFT48(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT47(Class, __VA_ARGS__)
#define FD_LEFT47(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT46(Class, __VA_ARGS__)
#define FD_LEFT46(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT45(Class, __VA_ARGS__)
#define FD_LEFT45(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT44(Class, __VA_ARGS__)
#define FD_LEFT44(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT43(Class, __VA_ARGS__)
#define FD_LEFT43(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT42(Class, __VA_ARGS__)
#define FD_LEFT42(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT41(Class, __VA_ARGS__)
#define FD_LEFT41(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT40(Class, __VA_ARGS__)
#define FD_LEFT40(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT39(Class, __VA_ARGS__)
#define FD_LEFT39(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT38(Class, __VA_ARGS__)
#define FD_LEFT38(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT37(Class, __VA_ARGS__)
#define FD_LEFT37(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT36(Class, __VA_ARGS__)
#define FD_LEFT36(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT35(Class, __VA_ARGS__)
#define FD_LEFT35(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT34(Class, __VA_ARGS__)
#define FD_LEFT34(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT33(Class, __VA_ARGS__)
#define FD_LEFT33(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT32(Class, __VA_ARGS__)
#define FD_LEFT32(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT31(Class, __VA_ARGS__)
#define FD_LEFT31(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT30(Class, __VA_ARGS__)
#define FD_LEFT30(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT29(Class, __VA_ARGS__)
#define FD_LEFT29(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT28(Class, __VA_ARGS__)
#define FD_LEFT28(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT27(Class, __VA_ARGS__)
#define FD_LEFT27(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT26(Class, __VA_ARGS__)
#define FD_LEFT26(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT25(Class, __VA_ARGS__)
#define FD_LEFT25(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT24(Class, __VA_ARGS__)
#define FD_LEFT24(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT23(Class, __VA_ARGS__)
#define FD_LEFT23(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT22(Class, __VA_ARGS__)
#define FD_LEFT22(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT21(Class, __VA_ARGS__)
#define FD_LEFT21(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT20(Class, __VA_ARGS__)
#define FD_LEFT20(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT19(Class, __VA_ARGS__)
#define FD_LEFT19(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT18(Class, __VA_ARGS__)
#define FD_LEFT18(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT17(Class, __VA_ARGS__)
#define FD_LEFT17(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT16(Class, __VA_ARGS__)
#define FD_LEFT16(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT15(Class, __VA_ARGS__)
#define FD_LEFT15(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT14(Class, __VA_ARGS__)
#define FD_LEFT14(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT13(Class, __VA_ARGS__)
#define FD_LEFT13(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT12(Class, __VA_ARGS__)
#define FD_LEFT12(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT11(Class, __VA_ARGS__)
#define FD_LEFT11(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT10(Class, __VA_ARGS__)
#define FD_LEFT10(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT9(Class, __VA_ARGS__)
#define FD_LEFT9(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT8(Class, __VA_ARGS__)
#define FD_LEFT8(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT7(Class, __VA_ARGS__)
#define FD_LEFT7(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT6(Class, __VA_ARGS__)
#define FD_LEFT6(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT5(Class, __VA_ARGS__)
#define FD_LEFT5(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT4(Class, __VA_ARGS__)
#define FD_LEFT4(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT3(Class, __VA_ARGS__)
#define FD_LEFT3(Class, f1, ...) FD_INSERT_ITM(Class, f1) FD_LEFT2(Class, __VA_ARGS__)
#define FD_LEFT2(Class, f1, ...) FD_INSERT_ITM(Class, f1)

#define FD_FIELD_L2R(Class, ...) FD_FIELD_L2R2(PP_NARG(Class, __VA_ARGS__), Class, __VA_ARGS__)
#define FD_FIELD_L2R2(n, Class, ...) FD_FIELD_LEFT(n, Class, __VA_ARGS__)
#define FD_FIELD_LEFT(n, Class, ...)          \
    FD_ONLY_FIRST_BEFORE(Class, __VA__ARGS__) \
    FD_LEFT##n(Class, __VA_ARGS__)            \
        FD_ONLY_FIRST_AFTER(Class, __VA_ARGS__)

#define FD_ONLY_FIRST_BEFORE(Class, ...)                                                           \
    namespace prism                                                                                \
    {                                                                                              \
    namespace reflection                                                                           \
    {                                                                                              \
    namespace privates                                                                             \
    {                                                                                              \
    template <>                                                                                    \
    struct st_field_do<Class>                                                                      \
    {                                                                                              \
        template <int BIS = 0, class LAM>                                                          \
        constexpr static bool run(Class& model, const char* fname, LAM&& lambda, int& level)       \
        {                                                                                          \
            ++level;                                                                               \
            using base_types = prism::reflection::privates::baseTypes<Class>::t;                   \
            if constexpr (!std::is_same_v<base_types, void>)                                       \
            {                                                                                      \
                bool hited = false;                                                                \
                prism::reflection::privates::for_each_bases<base_types>([&](auto* base_ptr_null) { \
                    using b_t = std::remove_pointer_t<decltype(base_ptr_null)>;                    \
                    b_t& baseRef = static_cast<b_t&>(model);                                       \
                    hited = st_field_do<b_t>::template run<BIS>(baseRef, fname, lambda, level);    \
                    --level;                                                                       \
                    if (hited)                                                                     \
                        return true;                                                               \
                    return false;                                                                  \
                });                                                                                \
                if (hited)                                                                         \
                    return true;                                                                   \
            }                                                                                      \
            switch (prism::utilities::const_hash(fname))                                           \
            {

#define FD_INSERT_ITM(Class, Field1)                                                                                                                  \
    case prism::utilities::const_hash(#Field1):                                                                                                       \
        if constexpr (!prism::attributes::privates::is_field_ignore<Class, decltype(prism::utilities::getMT(&Class::Field1)), &Class::Field1, BIS>()) \
        {                                                                                                                                             \
            lambda(model.Field1);                                                                                                                     \
            return true;                                                                                                                              \
        }                                                                                                                                             \
        break;

#define FD_ONLY_FIRST_AFTER(Class, Field1, ...) \
    }                                           \
    return false;                               \
    }                                           \
    }                                           \
    ;                                           \
    }                                           \
    }                                           \
    }

//静态迭代器
//
//
//

#define PRISM_FIELDS(...)     \
    FD_FIELD_L2R(__VA_ARGS__) \
    AT_FIELD_L2R(__VA_ARGS__) \
    FIELD_L2R(__VA_ARGS__)

#define LEFT130(Class, f1, ...) INSERT_ITM(Class, f1) LEFT129(Class, __VA_ARGS__)
#define LEFT129(Class, f1, ...) INSERT_ITM(Class, f1) LEFT128(Class, __VA_ARGS__)
#define LEFT128(Class, f1, ...) INSERT_ITM(Class, f1) LEFT127(Class, __VA_ARGS__)
#define LEFT127(Class, f1, ...) INSERT_ITM(Class, f1) LEFT126(Class, __VA_ARGS__)
#define LEFT126(Class, f1, ...) INSERT_ITM(Class, f1) LEFT125(Class, __VA_ARGS__)
#define LEFT125(Class, f1, ...) INSERT_ITM(Class, f1) LEFT124(Class, __VA_ARGS__)
#define LEFT124(Class, f1, ...) INSERT_ITM(Class, f1) LEFT123(Class, __VA_ARGS__)
#define LEFT123(Class, f1, ...) INSERT_ITM(Class, f1) LEFT122(Class, __VA_ARGS__)
#define LEFT122(Class, f1, ...) INSERT_ITM(Class, f1) LEFT121(Class, __VA_ARGS__)
#define LEFT121(Class, f1, ...) INSERT_ITM(Class, f1) LEFT120(Class, __VA_ARGS__)
#define LEFT120(Class, f1, ...) INSERT_ITM(Class, f1) LEFT119(Class, __VA_ARGS__)
#define LEFT119(Class, f1, ...) INSERT_ITM(Class, f1) LEFT118(Class, __VA_ARGS__)
#define LEFT118(Class, f1, ...) INSERT_ITM(Class, f1) LEFT117(Class, __VA_ARGS__)
#define LEFT117(Class, f1, ...) INSERT_ITM(Class, f1) LEFT116(Class, __VA_ARGS__)
#define LEFT116(Class, f1, ...) INSERT_ITM(Class, f1) LEFT115(Class, __VA_ARGS__)
#define LEFT115(Class, f1, ...) INSERT_ITM(Class, f1) LEFT114(Class, __VA_ARGS__)
#define LEFT114(Class, f1, ...) INSERT_ITM(Class, f1) LEFT113(Class, __VA_ARGS__)
#define LEFT113(Class, f1, ...) INSERT_ITM(Class, f1) LEFT112(Class, __VA_ARGS__)
#define LEFT112(Class, f1, ...) INSERT_ITM(Class, f1) LEFT111(Class, __VA_ARGS__)
#define LEFT111(Class, f1, ...) INSERT_ITM(Class, f1) LEFT110(Class, __VA_ARGS__)
#define LEFT110(Class, f1, ...) INSERT_ITM(Class, f1) LEFT109(Class, __VA_ARGS__)
#define LEFT109(Class, f1, ...) INSERT_ITM(Class, f1) LEFT108(Class, __VA_ARGS__)
#define LEFT108(Class, f1, ...) INSERT_ITM(Class, f1) LEFT107(Class, __VA_ARGS__)
#define LEFT107(Class, f1, ...) INSERT_ITM(Class, f1) LEFT106(Class, __VA_ARGS__)
#define LEFT106(Class, f1, ...) INSERT_ITM(Class, f1) LEFT105(Class, __VA_ARGS__)
#define LEFT105(Class, f1, ...) INSERT_ITM(Class, f1) LEFT104(Class, __VA_ARGS__)
#define LEFT104(Class, f1, ...) INSERT_ITM(Class, f1) LEFT103(Class, __VA_ARGS__)
#define LEFT103(Class, f1, ...) INSERT_ITM(Class, f1) LEFT102(Class, __VA_ARGS__)
#define LEFT102(Class, f1, ...) INSERT_ITM(Class, f1) LEFT101(Class, __VA_ARGS__)
#define LEFT101(Class, f1, ...) INSERT_ITM(Class, f1) LEFT100(Class, __VA_ARGS__)
#define LEFT100(Class, f1, ...) INSERT_ITM(Class, f1) LEFT99(Class, __VA_ARGS__)
#define LEFT99(Class, f1, ...) INSERT_ITM(Class, f1) LEFT98(Class, __VA_ARGS__)
#define LEFT98(Class, f1, ...) INSERT_ITM(Class, f1) LEFT97(Class, __VA_ARGS__)
#define LEFT97(Class, f1, ...) INSERT_ITM(Class, f1) LEFT96(Class, __VA_ARGS__)
#define LEFT96(Class, f1, ...) INSERT_ITM(Class, f1) LEFT95(Class, __VA_ARGS__)
#define LEFT95(Class, f1, ...) INSERT_ITM(Class, f1) LEFT94(Class, __VA_ARGS__)
#define LEFT94(Class, f1, ...) INSERT_ITM(Class, f1) LEFT93(Class, __VA_ARGS__)
#define LEFT93(Class, f1, ...) INSERT_ITM(Class, f1) LEFT92(Class, __VA_ARGS__)
#define LEFT92(Class, f1, ...) INSERT_ITM(Class, f1) LEFT91(Class, __VA_ARGS__)
#define LEFT91(Class, f1, ...) INSERT_ITM(Class, f1) LEFT90(Class, __VA_ARGS__)
#define LEFT90(Class, f1, ...) INSERT_ITM(Class, f1) LEFT89(Class, __VA_ARGS__)
#define LEFT89(Class, f1, ...) INSERT_ITM(Class, f1) LEFT88(Class, __VA_ARGS__)
#define LEFT88(Class, f1, ...) INSERT_ITM(Class, f1) LEFT87(Class, __VA_ARGS__)
#define LEFT87(Class, f1, ...) INSERT_ITM(Class, f1) LEFT86(Class, __VA_ARGS__)
#define LEFT86(Class, f1, ...) INSERT_ITM(Class, f1) LEFT85(Class, __VA_ARGS__)
#define LEFT85(Class, f1, ...) INSERT_ITM(Class, f1) LEFT84(Class, __VA_ARGS__)
#define LEFT84(Class, f1, ...) INSERT_ITM(Class, f1) LEFT83(Class, __VA_ARGS__)
#define LEFT83(Class, f1, ...) INSERT_ITM(Class, f1) LEFT82(Class, __VA_ARGS__)
#define LEFT82(Class, f1, ...) INSERT_ITM(Class, f1) LEFT81(Class, __VA_ARGS__)
#define LEFT81(Class, f1, ...) INSERT_ITM(Class, f1) LEFT80(Class, __VA_ARGS__)
#define LEFT80(Class, f1, ...) INSERT_ITM(Class, f1) LEFT79(Class, __VA_ARGS__)
#define LEFT79(Class, f1, ...) INSERT_ITM(Class, f1) LEFT78(Class, __VA_ARGS__)
#define LEFT78(Class, f1, ...) INSERT_ITM(Class, f1) LEFT77(Class, __VA_ARGS__)
#define LEFT77(Class, f1, ...) INSERT_ITM(Class, f1) LEFT76(Class, __VA_ARGS__)
#define LEFT76(Class, f1, ...) INSERT_ITM(Class, f1) LEFT75(Class, __VA_ARGS__)
#define LEFT75(Class, f1, ...) INSERT_ITM(Class, f1) LEFT74(Class, __VA_ARGS__)
#define LEFT74(Class, f1, ...) INSERT_ITM(Class, f1) LEFT73(Class, __VA_ARGS__)
#define LEFT73(Class, f1, ...) INSERT_ITM(Class, f1) LEFT72(Class, __VA_ARGS__)
#define LEFT72(Class, f1, ...) INSERT_ITM(Class, f1) LEFT71(Class, __VA_ARGS__)
#define LEFT71(Class, f1, ...) INSERT_ITM(Class, f1) LEFT70(Class, __VA_ARGS__)
#define LEFT70(Class, f1, ...) INSERT_ITM(Class, f1) LEFT69(Class, __VA_ARGS__)
#define LEFT69(Class, f1, ...) INSERT_ITM(Class, f1) LEFT68(Class, __VA_ARGS__)
#define LEFT68(Class, f1, ...) INSERT_ITM(Class, f1) LEFT67(Class, __VA_ARGS__)
#define LEFT67(Class, f1, ...) INSERT_ITM(Class, f1) LEFT66(Class, __VA_ARGS__)
#define LEFT66(Class, f1, ...) INSERT_ITM(Class, f1) LEFT65(Class, __VA_ARGS__)
#define LEFT65(Class, f1, ...) INSERT_ITM(Class, f1) LEFT64(Class, __VA_ARGS__)
#define LEFT64(Class, f1, ...) INSERT_ITM(Class, f1) LEFT63(Class, __VA_ARGS__)
#define LEFT63(Class, f1, ...) INSERT_ITM(Class, f1) LEFT62(Class, __VA_ARGS__)
#define LEFT62(Class, f1, ...) INSERT_ITM(Class, f1) LEFT61(Class, __VA_ARGS__)
#define LEFT61(Class, f1, ...) INSERT_ITM(Class, f1) LEFT60(Class, __VA_ARGS__)
#define LEFT60(Class, f1, ...) INSERT_ITM(Class, f1) LEFT59(Class, __VA_ARGS__)
#define LEFT59(Class, f1, ...) INSERT_ITM(Class, f1) LEFT58(Class, __VA_ARGS__)
#define LEFT58(Class, f1, ...) INSERT_ITM(Class, f1) LEFT57(Class, __VA_ARGS__)
#define LEFT57(Class, f1, ...) INSERT_ITM(Class, f1) LEFT56(Class, __VA_ARGS__)
#define LEFT56(Class, f1, ...) INSERT_ITM(Class, f1) LEFT55(Class, __VA_ARGS__)
#define LEFT55(Class, f1, ...) INSERT_ITM(Class, f1) LEFT54(Class, __VA_ARGS__)
#define LEFT54(Class, f1, ...) INSERT_ITM(Class, f1) LEFT53(Class, __VA_ARGS__)
#define LEFT53(Class, f1, ...) INSERT_ITM(Class, f1) LEFT52(Class, __VA_ARGS__)
#define LEFT52(Class, f1, ...) INSERT_ITM(Class, f1) LEFT51(Class, __VA_ARGS__)
#define LEFT51(Class, f1, ...) INSERT_ITM(Class, f1) LEFT50(Class, __VA_ARGS__)
#define LEFT50(Class, f1, ...) INSERT_ITM(Class, f1) LEFT49(Class, __VA_ARGS__)
#define LEFT49(Class, f1, ...) INSERT_ITM(Class, f1) LEFT48(Class, __VA_ARGS__)
#define LEFT48(Class, f1, ...) INSERT_ITM(Class, f1) LEFT47(Class, __VA_ARGS__)
#define LEFT47(Class, f1, ...) INSERT_ITM(Class, f1) LEFT46(Class, __VA_ARGS__)
#define LEFT46(Class, f1, ...) INSERT_ITM(Class, f1) LEFT45(Class, __VA_ARGS__)
#define LEFT45(Class, f1, ...) INSERT_ITM(Class, f1) LEFT44(Class, __VA_ARGS__)
#define LEFT44(Class, f1, ...) INSERT_ITM(Class, f1) LEFT43(Class, __VA_ARGS__)
#define LEFT43(Class, f1, ...) INSERT_ITM(Class, f1) LEFT42(Class, __VA_ARGS__)
#define LEFT42(Class, f1, ...) INSERT_ITM(Class, f1) LEFT41(Class, __VA_ARGS__)
#define LEFT41(Class, f1, ...) INSERT_ITM(Class, f1) LEFT40(Class, __VA_ARGS__)
#define LEFT40(Class, f1, ...) INSERT_ITM(Class, f1) LEFT39(Class, __VA_ARGS__)
#define LEFT39(Class, f1, ...) INSERT_ITM(Class, f1) LEFT38(Class, __VA_ARGS__)
#define LEFT38(Class, f1, ...) INSERT_ITM(Class, f1) LEFT37(Class, __VA_ARGS__)
#define LEFT37(Class, f1, ...) INSERT_ITM(Class, f1) LEFT36(Class, __VA_ARGS__)
#define LEFT36(Class, f1, ...) INSERT_ITM(Class, f1) LEFT35(Class, __VA_ARGS__)
#define LEFT35(Class, f1, ...) INSERT_ITM(Class, f1) LEFT34(Class, __VA_ARGS__)
#define LEFT34(Class, f1, ...) INSERT_ITM(Class, f1) LEFT33(Class, __VA_ARGS__)
#define LEFT33(Class, f1, ...) INSERT_ITM(Class, f1) LEFT32(Class, __VA_ARGS__)
#define LEFT32(Class, f1, ...) INSERT_ITM(Class, f1) LEFT31(Class, __VA_ARGS__)
#define LEFT31(Class, f1, ...) INSERT_ITM(Class, f1) LEFT30(Class, __VA_ARGS__)
#define LEFT30(Class, f1, ...) INSERT_ITM(Class, f1) LEFT29(Class, __VA_ARGS__)
#define LEFT29(Class, f1, ...) INSERT_ITM(Class, f1) LEFT28(Class, __VA_ARGS__)
#define LEFT28(Class, f1, ...) INSERT_ITM(Class, f1) LEFT27(Class, __VA_ARGS__)
#define LEFT27(Class, f1, ...) INSERT_ITM(Class, f1) LEFT26(Class, __VA_ARGS__)
#define LEFT26(Class, f1, ...) INSERT_ITM(Class, f1) LEFT25(Class, __VA_ARGS__)
#define LEFT25(Class, f1, ...) INSERT_ITM(Class, f1) LEFT24(Class, __VA_ARGS__)
#define LEFT24(Class, f1, ...) INSERT_ITM(Class, f1) LEFT23(Class, __VA_ARGS__)
#define LEFT23(Class, f1, ...) INSERT_ITM(Class, f1) LEFT22(Class, __VA_ARGS__)
#define LEFT22(Class, f1, ...) INSERT_ITM(Class, f1) LEFT21(Class, __VA_ARGS__)
#define LEFT21(Class, f1, ...) INSERT_ITM(Class, f1) LEFT20(Class, __VA_ARGS__)
#define LEFT20(Class, f1, ...) INSERT_ITM(Class, f1) LEFT19(Class, __VA_ARGS__)
#define LEFT19(Class, f1, ...) INSERT_ITM(Class, f1) LEFT18(Class, __VA_ARGS__)
#define LEFT18(Class, f1, ...) INSERT_ITM(Class, f1) LEFT17(Class, __VA_ARGS__)
#define LEFT17(Class, f1, ...) INSERT_ITM(Class, f1) LEFT16(Class, __VA_ARGS__)
#define LEFT16(Class, f1, ...) INSERT_ITM(Class, f1) LEFT15(Class, __VA_ARGS__)
#define LEFT15(Class, f1, ...) INSERT_ITM(Class, f1) LEFT14(Class, __VA_ARGS__)
#define LEFT14(Class, f1, ...) INSERT_ITM(Class, f1) LEFT13(Class, __VA_ARGS__)
#define LEFT13(Class, f1, ...) INSERT_ITM(Class, f1) LEFT12(Class, __VA_ARGS__)
#define LEFT12(Class, f1, ...) INSERT_ITM(Class, f1) LEFT11(Class, __VA_ARGS__)
#define LEFT11(Class, f1, ...) INSERT_ITM(Class, f1) LEFT10(Class, __VA_ARGS__)
#define LEFT10(Class, f1, ...) INSERT_ITM(Class, f1) LEFT9(Class, __VA_ARGS__)
#define LEFT9(Class, f1, ...) INSERT_ITM(Class, f1) LEFT8(Class, __VA_ARGS__)
#define LEFT8(Class, f1, ...) INSERT_ITM(Class, f1) LEFT7(Class, __VA_ARGS__)
#define LEFT7(Class, f1, ...) INSERT_ITM(Class, f1) LEFT6(Class, __VA_ARGS__)
#define LEFT6(Class, f1, ...) INSERT_ITM(Class, f1) LEFT5(Class, __VA_ARGS__)
#define LEFT5(Class, f1, ...) INSERT_ITM(Class, f1) LEFT4(Class, __VA_ARGS__)
#define LEFT4(Class, f1, ...) INSERT_ITM(Class, f1) LEFT3(Class, __VA_ARGS__)
#define LEFT3(Class, f1, ...) INSERT_ITM(Class, f1) LEFT2(Class, __VA_ARGS__)
#define LEFT2(Class, f1, ...) INSERT_ITM(Class, f1)

#define GET_2FIELD(f, f2, ...) f2
#define GET_1FIELD(f, ...) f

#define FIELD_L2R(Class, ...) FIELD_L2R2(PP_NARG(Class, __VA_ARGS__), Class, __VA_ARGS__)
#define FIELD_L2R2(n, Class, ...) FIELD_LEFT(n, Class, __VA_ARGS__)
#define FIELD_LEFT(n, Class, ...)          \
    ONLY_FIRST_BEFORE(Class, __VA__ARGS__) \
    LEFT##n(Class, __VA_ARGS__)            \
        ONLY_LAST_AFTER(Class, __VA_ARGS__)

#define ONLY_FIRST_BEFORE(Class, ...)                         \
    namespace prism                                           \
    {                                                         \
    namespace reflection                                      \
    {                                                         \
    namespace privates                                        \
    {                                                         \
    template <>                                               \
    struct st_visit_fields<Class>                             \
    {                                                         \
        template <int BIS, class Obj, class Vis>              \
        constexpr static void run(Obj&& model, Vis&& visitor) \
        {
#define INSERT_ITM(Class, Field1)                                                                                                                 \
    if constexpr (!prism::attributes::privates::is_field_ignore<Class, decltype(prism::utilities::getMT(&Class::Field1)), &Class::Field1, BIS>()) \
        visitor(#Field1, model.Field1);
#define ONLY_LAST_AFTER(Class, Field1, ...)                                                        \
    }                                                                                              \
    }                                                                                              \
    ;                                                                                              \
    template <>                                                                                    \
    struct st_for_each_fields<Class>                                                               \
    {                                                                                              \
        template <int BIS = 0, class LAM>                                                          \
        constexpr static void run(Class& model, LAM&& lambda)                                      \
        {                                                                                          \
            using base_types = prism::reflection::privates::baseTypes<Class>::t;                   \
            if constexpr (!std::is_same_v<base_types, void>)                                       \
            {                                                                                      \
                prism::reflection::privates::for_each_bases<base_types>([&](auto* base_ptr_null) { \
                    using b_t = std::remove_pointer_t<decltype(base_ptr_null)>;                    \
                    b_t& baseRef = static_cast<b_t&>(model);                                       \
                    prism::reflection::for_each_fields<BIS>(baseRef, lambda);                      \
                    return false;                                                                  \
                });                                                                                \
            }                                                                                      \
            st_visit_fields<Class>::run<BIS>(std::move(model), std::move(lambda));                 \
        }                                                                                          \
    };                                                                                             \
    }                                                                                              \
    template <>                                                                                    \
    constexpr bool has_md<Class>()                                                                 \
    {                                                                                              \
        return true;                                                                               \
    }                                                                                              \
    }                                                                                              \
    }

// base type
#define PRISM_CLASS_BASE_TYPES_FIRST_BEFORE(Class, Field1, ...) \
    namespace prism                                             \
    {                                                           \
    namespace reflection                                        \
    {                                                           \
    namespace privates                                          \
    {                                                           \
    template <>                                                 \
    struct baseTypes<Class>                                     \
    {                                                           \
        using t = prism::reflection::privates::typeList < Field1
#define PRISM_CLASS_BASE_TYPES_LEFT5(Class, f1, ...) , f1 PRISM_CLASS_BASE_TYPES_LEFT4(PRISM_CLASS_BASE_TYPES_GET_1FIELD(__VA_ARGS__), __VA_ARGS__)
#define PRISM_CLASS_BASE_TYPES_LEFT4(Class, f1, ...) , f1 PRISM_CLASS_BASE_TYPES_LEFT3(PRISM_CLASS_BASE_TYPES_GET_1FIELD(__VA_ARGS__), __VA_ARGS__)
#define PRISM_CLASS_BASE_TYPES_LEFT3(Class, f1, ...) , f1 PRISM_CLASS_BASE_TYPES_LEFT2(PRISM_CLASS_BASE_TYPES_GET_1FIELD(__VA_ARGS__), __VA_ARGS__)
#define PRISM_CLASS_BASE_TYPES_LEFT2(f1, ...) , f1
#define PRISM_CLASS_BASE_TYPES_LEFT1(f1, ...)
#define PRISM_CLASS_BASE_TYPES_GET_1FIELD(f, ...) f
#define PRISM_CLASS_BASE_TYPES_LAST_AFTER(Class, Field1, ...) \
    > ;                                                       \
    }                                                         \
    ;                                                         \
    }                                                         \
    }                                                         \
    }

#define PRISM_CLASS_BASE_TYPES_LEFT(n, Class, f1, ...)                                          \
    PRISM_CLASS_BASE_TYPES_FIRST_BEFORE(Class, f1, __VA__ARGS__)                                \
    PRISM_CLASS_BASE_TYPES_LEFT##n(PRISM_CLASS_BASE_TYPES_GET_1FIELD(__VA_ARGS__), __VA_ARGS__) \
        PRISM_CLASS_BASE_TYPES_LAST_AFTER(Class, __VA_ARGS__)
#define PRISM_CLASS_BASE_TYPES_L2R2(n, Class, f1, ...) PRISM_CLASS_BASE_TYPES_LEFT(n, Class, f1, __VA_ARGS__)
#define PRISM_CLASS_BASE_TYPES_L2R(Class, ...) PRISM_CLASS_BASE_TYPES_L2R2(PP_NARG(__VA_ARGS__), Class, __VA_ARGS__)
#define PRISM_CLASS_BASE_TYPES(...) PRISM_CLASS_BASE_TYPES_L2R(__VA_ARGS__)

namespace prism
{
namespace enums
{
template <class T, class = void>
struct enum_info
{
    constexpr static bool undef = true;
};
struct compareCharPointers
{
    bool operator()(const char* a, const char* b) const
    {
        if (a == nullptr && b == nullptr)
            return 0;
        else if (a != nullptr && b != nullptr)
            return std::strcmp(a, b) < 0;
        else if (a == nullptr && b != nullptr)
            return std::strcmp("", b) < 0;
        else if (a != nullptr && b == nullptr)
            return std::strcmp(a, "") < 0;

        return false;
    }
};
} // namespace enums

namespace attributes
{


namespace privates
{
template <class T, class M, M T::*fn, int>
constexpr inline bool is_field_ignore()
{
    return false;
}
// attributes of class
template <class T, class AT>
struct class_attribute
{
    constexpr static inline std::stdoptional<typename AT::value_type> value()
    {
        return std::nullopt;
    }
};
// attributes of field
template <class T, class M, M T::*fn, class AT>
struct field_attribute
{
    constexpr static std::stdoptional<typename AT::value_type> value()
    {
        return std::stdnullopt;
    }
};

template <class T, class AT>
struct st_field_attribute_do
{
    template <class LAM>
    constexpr static void run(const char* fname, LAM&& lambda) = delete;
};

} // namespace privates


template <class T,class AT>
constexpr static inline typename std::optional<typename AT::value_type> getFieldAttr(const char* fname)
{
    std::optional<typename AT::value_type> result = std::nullopt;
    prism::attributes::privates::st_field_attribute_do<T, AT>::run(fname, [&](std::optional<typename AT::value_type>&& attr) {
        result = attr;
    });
    return result;
}

template <class T,class AT>
constexpr static inline typename std::optional<typename AT::value_type> getClassAttr()
{
    return privates::class_attribute<T,AT>::value();
}

} // namespace attributes

namespace reflection
{
namespace privates
{

template <class T, class = void>
struct baseTypes
{
    using t = void;
};

template <typename...>
struct typeList
{
};

template <typename...>
struct typeListVisitor
{
};

template <template <typename...> class Sequence, typename... Args>
struct typeListVisitor<Sequence<Args...>>
{
    template <typename F>
    static constexpr inline bool run(F&& lam)
    {
        return DoVisit<F, Args...>(lam);
    }

    template <typename F>
    static constexpr inline bool DoVisit([[maybe_unused]] F&& lam)
    {
        return false;
    }

    template <typename F, typename Head>
    static constexpr inline bool DoVisit(F&& lam)
    {
        Head* p = nullptr;
        return lam(p);
    }

    template <typename F, typename Head, typename Next, typename... Tail>
    static constexpr inline bool DoVisit(F&& lam)
    {
        if (DoVisit<F, Head>(lam))
            return true;
        return DoVisit<F, Next, Tail...>(lam);
    }
};

template <typename Sequence, typename F>
constexpr static inline bool for_each_bases(F&& lam)
{
    return typeListVisitor<Sequence>::template run(lam);
}

template <class T>
struct st_visit_fields
{
};

template <class T>
struct st_field_do
{
    template <int BIS = 0, class LAM>
    constexpr static bool run([[maybe_unused]] T& model, [[maybe_unused]] const char* fname, [[maybe_unused]] LAM&& lambda, [[maybe_unused]] int& level)
    {
        return false;
    }
};

template <class T>
struct st_for_each_fields
{
    template <int BIS = 0, class LAM>
    constexpr static void run([[maybe_unused]] T& model, [[maybe_unused]] LAM&& lambda)
    {
    }
};

} // namespace privates

template <int BIS = 0, class T, class F>
constexpr static bool field_do(T& model, const char* fname, F&& lam)
{
    int level = 0;
    return privates::st_field_do<T>::template run<BIS>(model, fname, lam, level);
}

template <int BIS = 0, class T, class F>
constexpr void for_each_fields(T& model, F&& lam)
{
    privates::st_for_each_fields<T>::template run<BIS>(model, lam);
}

template <class>
constexpr bool has_md()
{
    return false;
}

template <class T>
std::enable_if_t<has_md<T>(), void> copy(T& left, T& right)
{
    for_each_fields(left, [&](const char* fname, auto&& lvalue) {
        field_do<T>(right, fname, [&](auto&& rvalue) {
            if constexpr (std::is_same_v<decltype(lvalue), decltype(rvalue)>)
            {
                lvalue = rvalue;
            }
        });
    });
}

} // namespace reflection

namespace utilities
{

template <typename Test, template <typename...> class Ref>
struct is_specialization : std::false_type
{
    //
};
template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type
{
};

unsigned constexpr inline const_hash(char const* input)
{
    return *input ? static_cast<unsigned int>(*input) + 33 * const_hash(input + 1) : 5381;
}

template <class T, class M>
constexpr static T getT(M T::*)
{
    return *(T*)0;
}
template <class T, class M>
constexpr static M getMT(M T::*)
{
    return *(M*)0;
}

template <class T>
struct has_def
{
    template <typename U>
    static auto test(U* p) -> decltype(U::undef, std::false_type{});
    template <typename U>
    static auto test(...) -> std::true_type;
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

} // namespace utilities

} // namespace prism

#endif // PRISM_PRISM_H
