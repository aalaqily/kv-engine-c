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
build config=config *args: _clean_hook
    cmake --build --preset {{preset}} --config {{config}} {{verbose_flag}} {{args}}

# Build project with Release config
build-release *args: (build "Release" args)

# Build unit tests executable
build-unit-tests config=config *args: (build config "--target" "unit_tests" args)

# Run CTest suite using preset and -C flag for config
test-unit-tests config=config *args: (build-unit-tests config args)
    ctest --preset {{preset}} -C {{config}} {{verbose_flag}} {{args}}

test-valgrind config=config *args: (build-unit-tests config)
    valgrind {{args}} build/{{preset}}/{{config}}/unit_tests

test *args: (test-unit-tests config args) (test-valgrind config)
