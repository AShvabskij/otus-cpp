cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

option(WITH_GOOGLE_TEST "Whether to build Google test" ON)

project(parser VERSION ${PROJECT_VERSION})

# ==================== Основной исполняемый файл ====================
add_executable(bulk
    main.cpp
    parser.h
    parser.cpp
    handlers.h
    handlers.cpp
    data.h
)

set_target_properties(bulk PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

# ==================== CPack для сборки пакетов ====================
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

# ==================== Google Test ====================
if(WITH_GOOGLE_TEST)

    enable_testing()

    find_package(GTest CONFIG REQUIRED)

    if(GTest_FOUND)
        message(STATUS "Google Test found!")
        message(STATUS "GTest include: ${GTest_INCLUDE_DIRS}")
        message(STATUS "GTest libraries: ${GTest_LIBRARIES}")
    endif()

    add_executable(bulk_tests
        tests/test_parser.cpp
        tests/test_handlers.cpp
    )

    target_link_libraries(bulk_tests PRIVATE
        GTest::gtest_main
    )

    target_compile_options(bulk_tests PRIVATE)

    include(GoogleTest)
    gtest_discover_tests(bulk_tests)
endif()
