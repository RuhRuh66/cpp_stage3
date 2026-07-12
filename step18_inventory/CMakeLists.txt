cmake_minimum_required(VERSION 3.16)

project(inventory CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(inventory
    src/main.cpp
    src/inventory.cpp
    )

target_include_directories(inventory PRIVATE include)


add_executable(inventory_tests 
    src/inventory.cpp
    tests/test_inventory.cpp)

target_include_directories(inventory_tests PRIVATE include)

enable_testing()

add_test(
    NAME inventory_tests
    COMMAND inventory_tests
)