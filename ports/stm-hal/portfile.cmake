# For overlay-ports, source is already in the port directory
set(SOURCE_PATH "${CURRENT_PORT_DIR}")
set(BUILD_DIR "${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-rel")

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

# Select target SKU from vcpkg features.
set(STM_HAL_SKU "STM32U031")
if("stm32u031" IN_LIST FEATURES AND "stm32u083" IN_LIST FEATURES)
  message(
    FATAL_ERROR
      "Features 'stm32u031' and 'stm32u083' are mutually exclusive for port '${PORT}'."
  )
elseif("stm32u083" IN_LIST FEATURES)
  set(STM_HAL_SKU "STM32U083")
endif()

# Determine generator based on target
if(VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Generic")
  set(GENERATOR_ARG -G "Ninja")
  set(TOOLCHAIN_ARG -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE})
else()
  set(GENERATOR_ARG "")
  set(TOOLCHAIN_ARG "")
endif()

# Avoid stale CMake cache when port folder names/paths change locally.
file(REMOVE_RECURSE "${BUILD_DIR}")

# Configure and build using CMake directly
execute_process(
  COMMAND
    "${CMAKE_COMMAND}" ${GENERATOR_ARG} -S "${SOURCE_PATH}" -B "${BUILD_DIR}"
    -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CURRENT_PACKAGES_DIR}
    -DCMAKE_PREFIX_PATH=${CURRENT_INSTALLED_DIR} ${TOOLCHAIN_ARG}
    -DCMAKE_CXX_SCAN_FOR_MODULES=OFF -DSTM_HAL_SKU=${STM_HAL_SKU}
    ${VCPKG_CMAKE_CONFIGURE_OPTIONS}
  RESULT_VARIABLE result)

# Build
execute_process(COMMAND "${CMAKE_COMMAND}" --build "${BUILD_DIR}" --config
                        Release RESULT_VARIABLE result)

# Install
execute_process(COMMAND "${CMAKE_COMMAND}" --install "${BUILD_DIR}" --config
                        Release RESULT_VARIABLE result)

# Remove debug folder
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

# Install copyright
file(
  INSTALL "${SOURCE_PATH}/vcpkg.json"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright)
