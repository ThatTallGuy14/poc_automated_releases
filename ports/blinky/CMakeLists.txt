cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(
  blinky
  VERSION 0.1.0
  LANGUAGES CXX)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Find the hal-interface dependency
find_package(hal-interface CONFIG REQUIRED)

# Create the library (not executable)
add_library(${PROJECT_NAME} STATIC)

# Add source files
target_sources(${PROJECT_NAME}
               PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/blinky_feature.cpp)

# Link against dependencies
if(TARGET hal-interface::hal-interface)
  target_link_libraries(${PROJECT_NAME} PUBLIC hal-interface::hal-interface)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC hal-interface)
endif()

# Include directories - make public so consumers get them
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                         $<INSTALL_INTERFACE:include>)

# Installation rules
install(
  TARGETS ${PROJECT_NAME}
  EXPORT blinky-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

# Install public headers - namespaced under blinky/
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/blinky
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

# Export targets
install(
  EXPORT blinky-targets
  FILE blinky-targets.cmake
  NAMESPACE blinky::
  DESTINATION share/blinky)

# Create and install package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/blinky-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/blinky-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/blinky-config.cmake"
  INSTALL_DESTINATION share/blinky)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/blinky-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/blinky-config-version.cmake"
        DESTINATION share/blinky)
