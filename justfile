default: workflow

preset := "ninja-multi"
config := "Debug"
clean := "0"
verbose := "0"

verbose_flag := if verbose == "1" { "--verbose" } else { "" }

cmake_args := ""
ctest_args := ""
valgrind_args := "" 

_clean_hook:
    @if [ "{{clean}}" = "1" ]; then rm -rf build/; fi

# Run CMake workflow preset
workflow: _clean_hook
    cmake --workflow --preset {{preset}} {{cmake_args}}

# Configure project
configure: _clean_hook
    cmake --preset {{preset}} --config {{config}} {{verbose_flag}} {{cmake_args}}

# Build project
build: _clean_hook
    cmake --build --preset {{preset}} --config {{config}} {{verbose_flag}} {{cmake_args}}

# Build project with Release config
build-release:
    just config="Release" build {{cmake_args}}

# Build unit tests executable
build-unit-tests: 
    just cmake_args="--target hashmap_unit_tests db_unit_tests {{cmake_args}}" build

# Run CTest suite using preset and -C flag for config
test-unit-tests: build-unit-tests
    ctest --preset {{preset}} -C {{config}} {{verbose_flag}} {{ctest_args}}

test-valgrind: build-unit-tests
    valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=1 {{valgrind_args}} build/{{preset}}/{{config}}/hashmap_unit_tests
    valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=1 {{valgrind_args}} build/{{preset}}/{{config}}/db_unit_tests

test: test-unit-tests test-valgrind
