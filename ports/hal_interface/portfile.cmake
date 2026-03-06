# Install headers directly
file(
  INSTALL "${CMAKE_CURRENT_LIST_DIR}/include/"
  DESTINATION "${CURRENT_PACKAGES_DIR}/include"
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

# Create and install CMake config file
file(
  WRITE "${CURRENT_PACKAGES_DIR}/share/hal-interface/hal-interface-config.cmake"
  "
include(CMakeFindDependencyMacro)
if(NOT TARGET hal-interface::hal-interface)
    add_library(hal-interface::hal-interface INTERFACE IMPORTED)
    target_include_directories(hal-interface::hal-interface INTERFACE \"\${CMAKE_CURRENT_LIST_DIR}/../../include\")
endif()
")

# Install vcpkg.json as copyright
file(
  INSTALL "${CMAKE_CURRENT_LIST_DIR}/vcpkg.json"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/hal-interface"
  RENAME copyright)
