cmake_minimum_required(VERSION 3.20)
project(kv-engine VERSION 0.0.0 LANGUAGES C)

# Force standard C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Fetch External dependency (xxHash)
include(FetchContent)

FetchContent_Declare(
    xxhash
    GIT_REPOSITORY https://github.com/Cyan4973/xxHash.git
    GIT_TAG v0.8.2
)

# Download and make xxHash available as a cmake target
FetchContent_MakeAvailable(xxhash)

# Core library target
add_library(kv-engine-core
    src/hashmap.c
)

# Alias target for cleaner internal consumption
add_library(kv-engine::core ALIAS kv-engine-core)

# Include directories
# BUILD_INTERFACE allows internal source files to find include/hashmap.h
# INSTALL_INTERFACE makes header paths work cleanly when installed
target_include_directories(kv-engine-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link xxHash to kv-engine-core
# xxHash exports the target `xxhash::xxhash`
target_link_libraries(kv-engine-core PRIVATE xxhash)

# Compiler warning options
target_compile_options(kv-engine-core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
