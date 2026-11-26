cmake_minimum_required(VERSION 3.20)

project(Lift LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(Lift src/main.cpp)


enable_testing()

find_package(GTest REQUIRED)

add_executable(LiftTests
    tests/test_command.cpp
)

target_include_directories(LiftTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(LiftTests PRIVATE
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(LiftTests)