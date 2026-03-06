cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable C++ module scanning only for native builds (not cross-compilation)
if(NOT CMAKE_CROSSCOMPILING)
  set(CMAKE_CXX_SCAN_FOR_MODULES ON)
else()
  set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
endif()

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME stm-hal)

# NOTE: Toolchain file should be provided externally via CMAKE_TOOLCHAIN_FILE
# For ARM builds, use the gcc-arm-none-eabi.cmake toolchain For native/test
# builds, use the default host compiler

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings`
project(
  ${CMAKE_PROJECT_NAME}
  VERSION 0.0.0
  LANGUAGES C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable C, C++ and ASM languages

# Find the hal-interface dependency First try to find it as an installed package
# (vcpkg builds)
find_package(hal-interface CONFIG QUIET)

# If not found, add it as a subdirectory (direct builds)
if(NOT hal-interface_FOUND)
  message(STATUS "hal-interface not found, adding as subdirectory")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../hal_interface
                   ${CMAKE_CURRENT_BINARY_DIR}/hal_interface)
endif()

# Create a single unified static library
add_library(${CMAKE_PROJECT_NAME} STATIC)

# STM32 HAL Driver sources (only the ones needed)
set(STM32_HAL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Src/stm32u0xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32u0xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32u0xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/system_stm32u0xx.c)

# Apollo wrapper driver sources
file(GLOB_RECURSE APOLLO_DRIVER_SOURCES "src/*.cpp" "src/*.c")

# Add all sources to the library
target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${STM32_HAL_SOURCES}
                                             ${APOLLO_DRIVER_SOURCES})

# Link against hal-interface (use namespaced target if available, otherwise
# plain target)
if(TARGET hal-interface::hal-interface)
  target_link_libraries(${CMAKE_PROJECT_NAME}
                        PUBLIC hal-interface::hal-interface)
else()
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC hal-interface)
endif()

# Include directories for the library
target_include_directories(
  ${CMAKE_PROJECT_NAME}
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Inc
          ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
          ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Inc/Legacy
          ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32U0xx/Include
          ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include)

# Compiler definitions
target_compile_definitions(
  ${CMAKE_PROJECT_NAME} PRIVATE USE_HAL_DRIVER STM32U031xx
                                $<$<CONFIG:Debug>:DEBUG>)

# Installation rules
install(
  TARGETS ${CMAKE_PROJECT_NAME}
  EXPORT stm-hal-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

# Install public headers - namespaced under stm_hal/
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/stm_hal
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

# Install STM32 HAL headers needed by the public interface
install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32U0xx_HAL_Driver/Inc/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h")

install(
  DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32U0xx/Include/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h")

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h")

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/
  DESTINATION include
  FILES_MATCHING
  PATTERN "*.h")

# Export targets
install(
  EXPORT stm-hal-targets
  FILE stm-hal-targets.cmake
  NAMESPACE stm-hal::
  DESTINATION share/stm-hal)

# Create and install package configuration file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/stm-hal-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/stm-hal-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/stm-hal-config.cmake"
  INSTALL_DESTINATION share/stm-hal)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/stm-hal-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/stm-hal-config-version.cmake"
        DESTINATION share/stm-hal)

# Install linker scripts and startup code for firmware builds
install(
  FILES "${CMAKE_CURRENT_SOURCE_DIR}/STM32U031xx_FLASH.ld"
        "${CMAKE_CURRENT_SOURCE_DIR}/STM32U031xx_RAM.ld"
        "${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32u031xx.s"
  DESTINATION share/stm-hal)
