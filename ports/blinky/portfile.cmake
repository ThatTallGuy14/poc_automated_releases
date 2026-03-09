set(SOURCE_PATH "${CURRENT_PORT_DIR}")

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

set(BLINKY_CMAKE_OPTIONS)
if(VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Generic")
  list(APPEND BLINKY_CMAKE_OPTIONS
       "-DCMAKE_TOOLCHAIN_FILE=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}")
endif()

if(COMMAND vcpkg_cmake_configure)
  vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}" OPTIONS -DCMAKE_CXX_SCAN_FOR_MODULES=OFF
    ${BLINKY_CMAKE_OPTIONS} ${VCPKG_CMAKE_CONFIGURE_OPTIONS})

  vcpkg_cmake_install()
  vcpkg_cmake_config_fixup(PACKAGE_NAME blinky CONFIG_PATH share/blinky)
elseif(COMMAND vcpkg_configure_cmake)
  vcpkg_configure_cmake(
    SOURCE_PATH
    "${SOURCE_PATH}"
    PREFER_NINJA
    OPTIONS
    -DCMAKE_CXX_SCAN_FOR_MODULES=OFF
    ${BLINKY_CMAKE_OPTIONS}
    ${VCPKG_CMAKE_CONFIGURE_OPTIONS})

  vcpkg_install_cmake()
  vcpkg_fixup_cmake_targets(CONFIG_PATH share/blinky)
else()
  message(FATAL_ERROR "No supported vcpkg CMake helper found for blinky port.")
endif()

# Keep only one copy of headers/CMake metadata in the package root.
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

file(
  INSTALL "${SOURCE_PATH}/vcpkg.json"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright)
