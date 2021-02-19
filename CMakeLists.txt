cmake_minimum_required(VERSION 3.14)

project(EZ_ITERATOR
VERSION 0.1)

include(GNUInstallDirs)
find_package(ez-cmake CONFIG REQUIRED)

add_library(ez-iterator INTERFACE)
target_compile_features(ez-iterator INTERFACE cxx_std_17)
target_include_directories(ez-iterator INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:include>")

set_target_properties(ez-iterator PROPERTIES EXPORT_NAME "iterator")

add_subdirectory("test")

set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-iterator)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
	TYPE INCLUDE
	FILES_MATCHING
	PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ez-iterator
	EXPORT ez-iterator-targets
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/$<CONFIG>"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/$<CONFIG>"
)

install_package(
	NAME "ez-iterator"
	NAMESPACE "ez::"
	DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/ez-iterator"
	EXPORT "ez-iterator-targets"
	VERSION "${EZ_ITERATOR_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	ARCH_INDEPENDENT
)