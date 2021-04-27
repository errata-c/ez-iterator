cmake_minimum_required(VERSION 3.14)

find_package(fmt CONFIG REQUIRED)

add_executable(combined_tests "main.cpp" "adapt.cpp" "enumerations.cpp" "ranges.cpp")
target_link_libraries(combined_tests PRIVATE ez::iterator fmt::fmt)