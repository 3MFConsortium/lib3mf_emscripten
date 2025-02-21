cmake_minimum_required(VERSION 3.29)
project(lib3mfEmscriptenTest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)

# Include directories
include_directories(include)

# Define the static libraries
set(STATIC_LIBRARIES
        ${CMAKE_CURRENT_LIST_DIR}/libs/lib3mf.a
        ${CMAKE_CURRENT_LIST_DIR}/libs/libzip.a
        ${CMAKE_CURRENT_LIST_DIR}/libs/libz.a
        ${CMAKE_CURRENT_LIST_DIR}/libs/libbz2.a
        ${CMAKE_CURRENT_LIST_DIR}/libs/libssl.a
        ${CMAKE_CURRENT_LIST_DIR}/libs/libcrypto.a
)

# Add the executable
add_executable(lib3mfEmscriptenTest.js main.cpp)

# Apply exception handling flags for linking only
target_link_options(lib3mfEmscriptenTest.js PRIVATE
        "-s WASM=1"
        "-s EXPORT_ALL=1"
        "-s ALLOW_MEMORY_GROWTH=1"
        "-s MAXIMUM_MEMORY=4294967296"  # 4GB
        "-s INITIAL_MEMORY=1073741824" # 1GB
        "-s FORCE_FILESYSTEM=1"
        "-s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"
        "--bind"
        "-s MODULARIZE=1"
        "-s EXCEPTION_CATCHING_ALLOWED=['*']"
        "-s NO_DISABLE_EXCEPTION_CATCHING=1"
        "-s ASSERTIONS=1"
        "-s EM_LOG_LEVEL=DEBUG"
)

# Link the static libraries
target_link_libraries(lib3mfEmscriptenTest.js ${STATIC_LIBRARIES})

# Set the output file name
set_target_properties(lib3mfEmscriptenTest.js PROPERTIES
        OUTPUT_NAME "lib3mfEmscriptenTest"
        SUFFIX ".js"
)

# Print configuration message
message(STATUS "Configured lib3mfEmscriptenTest for Emscripten with embind support and exception handling.")