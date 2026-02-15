cmake_minimum_required(VERSION 3.14)
if(NOT DEFINED PRISM_VERSION)
    file(READ "${CMAKE_CURRENT_LIST_DIR}/vcpkg.json" _contents)
    string(JSON PRISM_VERSION GET "${_contents}" version)
endif()

project(prism VERSION ${PRISM_VERSION} LANGUAGES CXX)



SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT})
    set(VCPKG_ROOT $ENV{VCPKG_ROOT} CACHE STRING "VCPKG_ROOT" FORCE )
endif()
if(DEFINED ENV{QT_DIR})
    set(QT_DIR $ENV{QT_DIR} CACHE STRING "QT_DIR" FORCE )
endif()
if(DEFINED ENV{Qt6_DIR})
    set(Qt6_DIR $ENV{Qt6_DIR} CACHE STRING "Qt6_DIR" FORCE)
endif()


option(VCPKG_DISABLE_TEST "disable run test when vcpkg install " ON)

#
set(HEADERS
    include/prism/prism.hpp
    include/prism/prismJson.hpp
    include/prism/prismSql.hpp
    include/prism/utilities/typeName.hpp
    )


add_library(${PROJECT_NAME}  INTERFACE  ${HEADERS})
add_library(prism::prism  ALIAS  ${PROJECT_NAME})
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)



if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(-W -Wextra)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finput-charset=UTF-8")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /Zc:preprocessor)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()


#
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${CMAKE_SYSTEM_PROCESSOR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${CMAKE_SYSTEM_PROCESSOR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}/${CMAKE_SYSTEM_PROCESSOR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
        ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        #NAMESPACE prism::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})

    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/)



# vcpkg
if(EXISTS ${VCPKG_ROOT})
    execute_process( COMMAND git rev-parse --abbrev-ref HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_BRANCH OUTPUT_STRIP_TRAILING_WHITESPACE RESULT_VARIABLE GIT_BRANCH_RESULT )
    execute_process( COMMAND git rev-parse HEAD WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE GIT_HASH OUTPUT_STRIP_TRAILING_WHITESPACE )
    string(REPLACE "_" "-" PORT_NAME "${PROJECT_NAME}")
    configure_file(${PROJECT_SOURCE_DIR}/cmake/vcpkg.json.in ${VCPKG_ROOT}/ports/${PORT_NAME}/vcpkg.json @ONLY NEWLINE_STYLE LF)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/vcpkg.json.in ${PROJECT_SOURCE_DIR}/vcpkg.json @ONLY NEWLINE_STYLE LF)
    configure_file(${PROJECT_SOURCE_DIR}/cmake/vcpkg_cus_info.json.in ${VCPKG_ROOT}/ports/${PORT_NAME}/vcpkg_cus_info.json @ONLY NEWLINE_STYLE LF)
    file(COPY ${PROJECT_SOURCE_DIR}/cmake/portfile.cmake DESTINATION ${VCPKG_ROOT}/ports/${PORT_NAME})
endif()




# test

#    find_package(unofficial-sqlite3 CONFIG REQUIRED)
#    find_package(Catch2 REQUIRED)
#    include(CTest)
#    include(Catch)
#
#    #test prism json
#    set(TST_PRISM_SOURCE tests/test_prism.cpp tests/models/test_model.h)
#    add_executable(tst_${PROJECT_NAME} ${TST_PRISM_SOURCE} )
#    target_compile_features(tst_${PROJECT_NAME} PUBLIC cxx_std_17)
#    target_link_libraries(tst_${PROJECT_NAME} PRIVATE Catch2::Catch2  unofficial::sqlite3::sqlite3)
#    #catch_discover_tests(tst_${PROJECT_NAME})




#execute_process(COMMAND ${CMAKE_COMMAND} -E echo "
#----------
#----------" )
