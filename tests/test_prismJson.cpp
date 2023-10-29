#define CATCH_CONFIG_RUNNER
#include "include/prism/basic/prismJson.hpp"
#include "models/test_model.h"
#include <catch2/catch_all.hpp>

#ifdef _MSC_VER
#include <Windows.h>
#endif


int main(int argc, const char** argv)
{
#ifdef _MSC_VER
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    int result = Catch::Session().run(argc, argv);
    return result;
}
