cmake_minimum_required(VERSION 3.22)

project(
  hal-interface
  VERSION 0.1.0
  LANGUAGES CXX)

# Export compile commands for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard for C++20 modules support
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create interface library for the GPIO interface
add_library(hal-interface INTERFACE)
add_library(hal-interface::hal-interface ALIAS hal-interface)

# Set include directories
target_include_directories(
  hal-interface INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                          $<INSTALL_INTERFACE:include>)

# Install the library
install(TARGETS hal-interface EXPORT hal-interface-targets)

# Install headers - namespaced under hal-interface/
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/hal-interface
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

# Install export configuration
install(
  EXPORT hal-interface-targets
  FILE hal-interface-targets.cmake
  NAMESPACE hal-interface::
  DESTINATION share/hal-interface)

# Create and install package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/hal-interface-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/hal-interface-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/hal-interface-config.cmake"
  INSTALL_DESTINATION share/hal-interface)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/hal-interface-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/hal-interface-config-version.cmake"
        DESTINATION share/hal-interface)
