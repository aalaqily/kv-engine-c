default: build

# Create build dir
[arg('clean', pattern='--clean|')]
create clean="" *args:
    if [ -n '{{clean}}' ]; then rm build/ -rf; fi
    cmake -B build/ninja-multi -G "Ninja Multi-Config" {{args}}

# Duild (Debug config - Default)
build *args:
    cmake --build build/ninja-multi {{args}}

# Build (Release config)
build-release *args: (build "--config" "Release" args)

build-unit-tests *args: (build "--target" "unit_tests" args)

test-unit-tests *args: build-unit-tests
    ctest --test-dir build/ninja-multi -C Debug --output-on-failure {{args}}

test *args: (test-unit-tests args)
