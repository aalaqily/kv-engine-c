default: workflow

preset := "ninja-multi"
config := "Debug"
clean := "0"
verbose := "0"

verbose_flag := if verbose == "1" { "--verbose" } else { "" }

_clean_hook:
    @if [ "{{clean}}" = "1" ]; then rm -rf build/; fi

# Run CMake workflow preset
workflow *args: _clean_hook
    cmake --workflow --preset {{preset}} {{args}}

# Configure project
configure *args: _clean_hook
    cmake --preset {{preset}} --config {{config}} {{verbose_flag}} {{args}}

# Build project
build *args: _clean_hook
    cmake --build --preset {{preset}} --config {{config}} {{verbose_flag}} {{args}}

# Build project with Release config
build-release *args:
    just config="Release" build {{args}}

# Build unit tests executable
build-unit-tests *args: (build "--target" "hashmap_unit_tests" "db_unit_tests" args)

# Run CTest suite using preset and -C flag for config
test-unit-tests *args: (build-unit-tests args)
    ctest --preset {{preset}} -C {{config}} {{verbose_flag}} {{args}}

test-valgrind *args: (build-unit-tests)
    valgrind --leak-check=full --errors-for-leak-kinds=all --error-exitcode=1 {{args}} build/{{preset}}/{{config}}/unit_tests

test unit_tests_args="" valgrind_args="": (test-unit-tests unit_tests_args) (test-valgrind valgrind_args)
