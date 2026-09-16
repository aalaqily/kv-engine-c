cmake_minimum_required(VERSION 3.5)
project(kv-engine VERSION 0.1.0 LANGUAGES C)

# Force standard C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)

# Fetch External dependency (xxHash)
include(FetchContent)

FetchContent_Declare(
    xxhash
    GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
    GIT_TAG v0.8.2
)

FetchContent_MakeAvailable(xxhash)

FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/cofyc/argparse.git
    GIT_TAG 4e30aba50340af7f2d3932492c4bffdef7669db8
)

FetchContent_MakeAvailable(argparse)

# Apply warning flags to a target
function(kv_engine_enable_warnings target)
    target_compile_options(${target} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )
endfunction()

# Core library: hashmap + persistence. Internal to the engine; not exposed publicly.
add_library(kv_engine_core
    core/src/hashmap.c
    core/src/hashmap_iterator.c
    core/src/db.c
)
add_library(kv_engine::core ALIAS kv_engine_core)

target_include_directories(kv_engine_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/core/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(kv_engine_core PRIVATE xxhash)
kv_engine_enable_warnings(kv_engine_core)

# Public API layer: opaque KVEngine over the core. Core stays private to consumers.
add_library(kv_engine_api
    api/src/kv_engine.c
)
add_library(kv_engine::api ALIAS kv_engine_api)

target_include_directories(kv_engine_api PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(kv_engine_api PRIVATE kv_engine::core)
kv_engine_enable_warnings(kv_engine_api)

# Application executable
add_executable(kv_engine_app
    app/src/main.c
)
set_target_properties(kv_engine_app PROPERTIES OUTPUT_NAME "kv-engine")
target_link_libraries(kv_engine_app PRIVATE kv_engine::api argparse_static)
kv_engine_enable_warnings(kv_engine_app)

# Enable testing
enable_testing()

# Function to set up a unit test executable with asserts kept active (Release
# defines NDEBUG, which would otherwise compile out every assert).
function(kv_engine_add_unit_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE kv_engine::core)
    target_compile_options(${name} PRIVATE
        $<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-UNDEBUG>
    )
    kv_engine_enable_warnings(${name})
    add_test(NAME ${name} COMMAND ${name})
endfunction()

kv_engine_add_unit_test(hashmap_unit_tests core/unit_tests/test_hashmap.c)
kv_engine_add_unit_test(hashmap_iterator_unit_tests core/unit_tests/test_hashmap_iterator.c)
kv_engine_add_unit_test(db_unit_tests core/unit_tests/test_db.c)