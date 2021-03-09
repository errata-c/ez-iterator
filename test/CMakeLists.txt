cmake_minimum_required(VERSION 3.14)

find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_executable(combined_tests "main.cpp" "basic.cpp" "adapt.cpp")
target_link_libraries(combined_tests PRIVATE ez-iterator fmt::fmt Catch2::Catch2)