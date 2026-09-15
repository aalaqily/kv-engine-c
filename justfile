default: workflow

preset := "ninja-multi"
config := "Debug"
clean := "0"
verbose := "0"
cmake_args := ""
ctest_args := ""
valgrind_args := ""

all_args := "preset='" + preset + "' config='" + config + "' clean='" + clean + "' verbose='" + verbose + "' cmake_args='" + cmake_args + "' ctest_args='" + ctest_args + "' valgrind_args='" + valgrind_args + "'"

verbose_flag := if verbose == "1" { "--verbose" } else { "" }

# Remove the build directory
clean_build:
    rm -rf build/

_clean_hook:
    @if [ "{{clean}}" = "1" ]; then just clean_build; fi

# Configure, build and test via the CMake workflow preset
workflow: _clean_hook
    cmake --workflow --preset {{preset}} {{cmake_args}}

# Configure project
configure: _clean_hook
    cmake --preset {{preset}} {{cmake_args}}

# Build project
build: _clean_hook
    cmake --build --preset {{preset}} --config {{config}} {{verbose_flag}} {{cmake_args}}

# Build project with Release config
build-release:
    just {{all_args}} config="Release" build

# Build unit tests executables
build-unit-tests:
    just {{all_args}} cmake_args="--target hashmap_unit_tests db_unit_tests" build

# Build and run the kv_engine_app executable
run:
    just {{all_args}} cmake_args="--target kv_engine_app" build
    ./build/{{preset}}/{{config}}/kv_engine_app

# Run CTest suite using preset and -C flag for config
test-unit-tests: build-unit-tests
    ctest --preset {{preset}} -C {{config}} {{verbose_flag}} {{ctest_args}}

test-valgrind: build-unit-tests
    valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=1 {{valgrind_args}} build/{{preset}}/{{config}}/hashmap_unit_tests
    valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=1 {{valgrind_args}} build/{{preset}}/{{config}}/db_unit_tests

# Unit tests + leak checking
test: test-unit-tests test-valgrind

# Format all source files in place with clang-format
format:
    clang-format -i core/include/*.h core/src/*.c core/unit_tests/*.c api/include/*.h api/src/*.c app/src/*.c

# Check that all source files are properly formatted
format-check:
    clang-format --dry-run -Werror core/include/*.h core/src/*.c core/unit_tests/*.c api/include/*.h api/src/*.c app/src/*.c
