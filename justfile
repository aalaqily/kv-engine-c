default: workflow

preset := "ninja-multi"
config := "Debug"
clean := "0"

verbose := "0"
verbose_flag := if verbose == "1" { "--verbose" } else { "" }

memcheck := "0"
memcheck_command := if memcheck == "1" {"valgrind"} else { "" }
memcheck_args := "--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1"
memcheck_ctest_flag := if memcheck == "1" {"-T memcheck"} else { "" }

unit_tests := "hashmap_unit_tests hashmap_iterator_unit_tests db_unit_tests"

# Remove the build directory
clean-build:
    rm -rf build/

_clean-hook:
    @if [ "{{clean}}" = "1" ]; then just clean-build; fi

# Configure, build and test via the CMake workflow preset
workflow *args: _clean-hook
    cmake --workflow --preset {{preset}} {{args}}

# Configure project
configure *args: _clean-hook
    cmake --preset {{preset}} {{args}}

# Build project
build *args: _clean-hook
    cmake --build --preset {{preset}} --config {{config}} {{verbose_flag}} {{args}}

# Build project with Release config
build-release:
    just config="Release" build

# Build unit tests executables
build-tests: (build "--target" unit_tests)

# Build and run the kv_engine_app executable
run *args: (build "--target" "kv_engine_app")
    {{memcheck_command}} {{memcheck_args}} ./build/{{preset}}/{{config}}/kv-engine {{args}}

# Run unit tests, with Valgrind memcheck when valgrind=1
test *args: build-tests
    ctest --test-dir build/{{preset}} -C {{config}} {{verbose_flag}} {{memcheck_ctest_flag}} {{args}}
    if [ {{memcheck}} == 1 ]; then bat -P build/ninja-multi/Testing/Temporary/MemoryChecker.*.log; fi

# Format all source files in place with clang-format
format:
    clang-format -i core/include/*.h core/src/*.c core/unit_tests/*.c api/include/*.h api/src/*.c app/src/*.c

# Check that all source files are properly formatted
format-check:
    clang-format --dry-run -Werror core/include/*.h core/src/*.c core/unit_tests/*.c api/include/*.h api/src/*.c app/src/*.c
