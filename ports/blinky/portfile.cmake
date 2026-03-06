# Configure and build using CMake directly
set(SOURCE_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(BUILD_DIR "${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-rel")

# Determine generator based on target
if(VCPKG_CMAKE_SYSTEM_NAME STREQUAL "Generic")
  set(GENERATOR_ARG -G "Ninja")
  set(TOOLCHAIN_ARG -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE})
else()
  set(GENERATOR_ARG "")
  set(TOOLCHAIN_ARG "")
endif()

# Avoid stale CMake cache when registry git-tree paths change.
file(REMOVE_RECURSE "${BUILD_DIR}")

# Run CMake configure
execute_process(
  COMMAND
    "${CMAKE_COMMAND}" ${GENERATOR_ARG} -S "${SOURCE_PATH}" -B "${BUILD_DIR}"
    -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CURRENT_PACKAGES_DIR}
    -DCMAKE_PREFIX_PATH=${CURRENT_INSTALLED_DIR} ${TOOLCHAIN_ARG}
    ${VCPKG_CMAKE_CONFIGURE_OPTIONS}
  RESULT_VARIABLE result)
if(result)
  message(FATAL_ERROR "CMake configure failed")
endif()

# Build
execute_process(COMMAND "${CMAKE_COMMAND}" --build "${BUILD_DIR}" --config
                        Release RESULT_VARIABLE result)
if(result)
  message(FATAL_ERROR "CMake build failed")
endif()

# Install
execute_process(COMMAND "${CMAKE_COMMAND}" --install "${BUILD_DIR}" --config
                        Release RESULT_VARIABLE result)
if(result)
  message(FATAL_ERROR "CMake install failed")
endif()

# Remove debug files for header-only
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug")

# Install copyright
file(
  INSTALL "${SOURCE_PATH}/vcpkg.json"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright)
