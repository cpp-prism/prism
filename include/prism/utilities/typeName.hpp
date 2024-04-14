#ifndef PRISM_UTILITIES_TYPENAME_HPP
#define PRISM_UTILITIES_TYPENAME_HPP

#define _ENABLE_EXPERIMENTAL_OPTIONAL_

#ifndef EXPERIMENTAL_PRISM
#define EXPERIMENTAL_PRISM
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 7
#include <experimental/optional>
#include <experimental/string_view>
#define stdoptional experimental::optional
#define stdnullopt experimental::nullopt
#define stdstring_view experimental::string_view
#define GCCSUB7
#else
#include <optional>
#include <string_view>
#define stdoptional optional
#define stdnullopt nullopt
#define stdstring_view string_view
#endif
#endif



namespace prism::utilities{
template <typename T>
struct typeName
{
    constexpr static std::stdstring_view fullname_intern()
    {
#if defined(__clang__) || defined(__GNUC__)
        return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
    }
    constexpr static std::stdstring_view name()
    {
        size_t prefix_len = typeName<void>::fullname_intern().find("void");
        size_t multiple = typeName<void>::fullname_intern().size() - typeName<int>::fullname_intern().size();
        size_t dummy_len = typeName<void>::fullname_intern().size() - 4 * multiple;
        size_t target_len = (fullname_intern().size() - dummy_len) / multiple;
        std::stdstring_view rv = fullname_intern().substr(prefix_len, target_len);

        //if (rv.rfind(' ') == rv.npos)
        //    return rv;
        return rv.substr(rv.rfind(' ') + 1);
    }

    constexpr static std::stdstring_view value = name();
};

}//prism::utilities
#endif // PRISM_UTILITIES_TYPENAME_HPP
