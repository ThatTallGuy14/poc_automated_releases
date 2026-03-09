set(SOURCE_PATH "${CURRENT_PORT_DIR}")

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

# Select target SKU from vcpkg features.
set(STM_HAL_SKU "STM32U031")
if(("stm32u031" IN_LIST FEATURES AND "stm32u083" IN_LIST FEATURES)
   OR ("stm32u031" IN_LIST FEATURES AND "nucleo-u083rc" IN_LIST FEATURES)
   OR ("stm32u083" IN_LIST FEATURES AND "nucleo-u083rc" IN_LIST FEATURES))
  message(
    FATAL_ERROR
      "Features 'stm32u031', 'stm32u083', and 'nucleo-u083rc' are mutually exclusive for port '${PORT}'."
  )
elseif("nucleo-u083rc" IN_LIST FEATURES)
  set(STM_HAL_SKU "NUCLEO-U083RC")
elseif("stm32u083" IN_LIST FEATURES)
  set(STM_HAL_SKU "STM32U083")
endif()

set(STM_HAL_CMAKE_OPTIONS)
if(VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Generic")
  list(APPEND STM_HAL_CMAKE_OPTIONS
       "-DCMAKE_TOOLCHAIN_FILE=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}")
endif()

if(COMMAND vcpkg_cmake_configure)
  vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
      -DCMAKE_CXX_SCAN_FOR_MODULES=OFF
      -DSTM_HAL_SKU=${STM_HAL_SKU}
      ${STM_HAL_CMAKE_OPTIONS}
      ${VCPKG_CMAKE_CONFIGURE_OPTIONS})

  vcpkg_cmake_install()
  vcpkg_cmake_config_fixup(PACKAGE_NAME stm-hal CONFIG_PATH share/stm-hal)
elseif(COMMAND vcpkg_configure_cmake)
  vcpkg_configure_cmake(
    SOURCE_PATH "${SOURCE_PATH}"
    PREFER_NINJA
    OPTIONS
      -DCMAKE_CXX_SCAN_FOR_MODULES=OFF
      -DSTM_HAL_SKU=${STM_HAL_SKU}
      ${STM_HAL_CMAKE_OPTIONS}
      ${VCPKG_CMAKE_CONFIGURE_OPTIONS})

  vcpkg_install_cmake()
  vcpkg_fixup_cmake_targets(CONFIG_PATH share/stm-hal)
else()
  message(FATAL_ERROR "No supported vcpkg CMake helper found for stm-hal port.")
endif()

# Remove debug folder
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

# Install copyright
file(
  INSTALL "${SOURCE_PATH}/vcpkg.json"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright)
