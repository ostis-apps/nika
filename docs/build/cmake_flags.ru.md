# Флаги CMake

## Кэш сборки

Этот проект автоматически использует ccache, если он доступен в системе. Чтобы отключить это, используйте флаг `-DAUTO_CCACHE=OFF`.

## Сборка тестов

```sh
cmake --preset <configure-preset> -DSC_BUILD_TESTS=ON
cmake --build --preset <build-preset>
```

## Форматирование кода с помощью CLangFormat

Чтобы проверить код с помощью CLangFormat, выполните:

```sh
cmake --preset release-with-tests-conan -DSC_CLANG_FORMAT_CODE=ON
cmake --build --preset release --target clangformat_check
```

или

```sh
./scripts/clang/check_formatting.sh
```

Чтобы отформатировать код с помощью CLangFormat, выполните:

```sh
cmake --preset release-with-tests-conan -DSC_CLANG_FORMAT_CODE=ON
cmake --build --preset release --target clangformat
```

или

```sh
./scripts/clang/format_code.sh
```
