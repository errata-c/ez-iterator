cmake_minimum_required(VERSION 3.14)

find_package(fmt CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

add_executable(basic_test "basic.cpp")
target_link_libraries(basic_test PRIVATE ez-iterator fmt::fmt Catch2::Catch2)

add_executable(adapt_test "adapt.cpp")
target_link_libraries(adapt_test PRIVATE ez-iterator fmt::fmt-header-only)