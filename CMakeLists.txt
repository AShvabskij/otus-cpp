cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

project(matrix VERSION ${PROJECT_VESRION})

add_executable(matrix main.cpp
    matrix.h
    matrix_md.h)

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(matrix PRIVATE
        /W4
    )
else ()
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS matrix RUNTIME DESTINATION bin)

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

    if(GTest_FOUND)
        message(STATUS "Google Test found!")
        message(STATUS "GTest include: ${GTest_INCLUDE_DIRS}")
        message(STATUS "GTest libraries: ${GTest_LIBRARIES}")
    endif()

    add_executable(matrix_tests tests/test_matrix.cpp tests/test_matrix_md.cpp)

    target_link_libraries(matrix_tests PRIVATE
        GTest::gtest_main
    )

    target_compile_options(matrix_tests PRIVATE)

    include(GoogleTest)
    gtest_discover_tests(matrix_tests)
endif()
