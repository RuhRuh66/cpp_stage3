cmake_minimum_required(VERSION 3.16)

project(HttpRaii LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(curl_basic
    src/main.cpp)

target_link_libraries(curl_basic PRIVATE CURL::libcurl)
target_link_libraries(curl_basic PRIVATE nlohmann_json::nlohmann_json)