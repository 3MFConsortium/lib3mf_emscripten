cmake_minimum_required(VERSION 3.29)

# Use VCPKG toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

project(lib3mfEmscriptenTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

# Find lib3mf via VCPKG
find_package(lib3mf REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/include)
set(STATIC_LIBRARIES
        ${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/lib/lib3mf.a
        ${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/lib/libzip.a
        ${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/lib/libz.a
        ${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/lib/libbz2.a
        ${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/lib/libssl.a
        ${CMAKE_CURRENT_LIST_DIR}/vcpkg/installed/wasm32-emscripten/lib/libcrypto.a
)

# Add the executable
add_executable(lib3mfEmscriptenTest.js main.cpp)

# Apply Emscripten-specific linking options
target_link_options(lib3mfEmscriptenTest.js PRIVATE
        "-s WASM=1"
        "-s EXPORT_ALL=1"
        "-s ALLOW_MEMORY_GROWTH=1"
        "-s MAXIMUM_MEMORY=4294967296"  # 4GB
        "-s INITIAL_MEMORY=1073741824"  # 1GB
        "-s FORCE_FILESYSTEM=1"
        "-s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"
        "--bind"
        "-s MODULARIZE=1"
        "-s EXCEPTION_CATCHING_ALLOWED=['*']"
        "-s NO_DISABLE_EXCEPTION_CATCHING=1"
        "-s ASSERTIONS=1"
        "-s EM_LOG_LEVEL=DEBUG"
)

# Link lib3mf (VCPKG will handle all dependencies)
target_link_libraries(lib3mfEmscriptenTest.js PRIVATE ${STATIC_LIBRARIES})

# Set the output file name
set_target_properties(lib3mfEmscriptenTest.js PROPERTIES
        OUTPUT_NAME "lib3mfEmscriptenTest"
        SUFFIX ".js"
)

# Print configuration message
message(STATUS "Configured lib3mfEmscriptenTest for Emscripten using VCPKG.")