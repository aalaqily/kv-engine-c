cmake_minimum_required(VERSION 3.20)
project(kv-engine VERSION 0.0.0 LANGUAGES C)

# Force standard C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# 1. Core library target
add_library(kv-engine-core
    src/hashmap.c
)

# Alias target for cleaner internal consumption
add_library(kv-engine::core ALIAS kv-engine-core)

# 2. Include directories
# BUILD_INTERFACE allows internal source files to find include/hashmap.h
# INSTALL_INTERFACE makes header paths work cleanly when installed
target_include_directories(kv-engine-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 3. Compiler warning options
target_compile_options(kv-engine-core PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
