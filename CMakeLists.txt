cmake_minimum_required(VERSION 3.14)

project(EZ_ITERATOR
	VERSION 0.2.1
	LANGUAGES C CXX
)
include(GNUInstallDirs)
include("cmake/ez-cmake/install-package.cmake")

option(BUILD_TESTS "Build the test executables" ON)
option(SEPARATE_DIRECTORY "Install the package into its own directory" ON)
set(CONFIG_DIR "${CMAKE_INSTALL_DATAROOTDIR}/ez-iterator" CACHE STRING "The relative directory to install package config files.")



find_package(ez-meta CONFIG REQUIRED)

add_library(ez-iterator INTERFACE)
target_compile_features(ez-iterator INTERFACE cxx_std_17)
target_include_directories(ez-iterator INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:include>")
target_link_libraries(ez-iterator INTERFACE ez::meta)

set_target_properties(ez-iterator PROPERTIES EXPORT_NAME "iterator")

if(BUILD_TESTS)
	add_subdirectory("test")
endif()

if(SEPARATE_DIRECTORY)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/ez-iterator)
endif()

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
	DESTINATION "${CONFIG_DIR}"
	EXPORT "ez-iterator-targets"
	VERSION "${EZ_ITERATOR_VERSION}"
	COMPATIBILITY "SameMajorVersion"
	ARCH_INDEPENDENT
)