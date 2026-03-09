# For overlay-ports, source is already in the port directory
set(SOURCE_PATH "${CURRENT_PORT_DIR}")
set(BUILD_DIR "${CURRENT_BUILDTREES_DIR}/${TARGET_TRIPLET}-rel")

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

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
    -DCMAKE_CXX_SCAN_FOR_MODULES=OFF ${VCPKG_CMAKE_CONFIGURE_OPTIONS}
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

# Remove debug folder
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

# Install copyright
file(
  INSTALL "${SOURCE_PATH}/vcpkg.json"
  DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}"
  RENAME copyright)
