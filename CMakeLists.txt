cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VESRION})

option(WITH_GOOGLE_TEST "Whether to build Google test" OFF)

add_executable(ip_filter main.cpp
    func.h
    func.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

# ========== Google Test ==========
if(WITH_GOOGLE_TEST)

    enable_testing()

    find_package(GTest CONFIG REQUIRED)

    add_executable(ip_filter_test test_ip_filter.cpp func.cpp)

    target_link_libraries(ip_filter_test PRIVATE
        GTest::gtest_main
    )

    target_compile_options(ip_filter_test PRIVATE)

    include(GoogleTest)
    gtest_discover_tests(ip_filter_test)
endif()
