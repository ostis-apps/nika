# CMake Flags

## Build cache

This project uses ccache automatically if it's available in the system. To disable this, use flag `-DAUTO_CCACHE=OFF`.

## Building tests

```sh
cmake --preset <configure-preset> -DSC_BUILD_TESTS=ON
cmake --build --preset <build-preset>
```

## Code formatting with CLangFormat

To check code with CLangFormat run:
```sh
cmake --preset release-with-tests-conan -DSC_CLANG_FORMAT_CODE=ON
cmake --build --preset release --target clangformat_check
```

or
```sh
./scripts/clang/check_formatting.sh
```

To format code with CLangFormat run:
```sh
cmake --preset release-with-tests-conan -DSC_CLANG_FORMAT_CODE=ON
cmake --build --preset release --target clangformat
```

or
```sh
./scripts/clang/format_code.sh
```
