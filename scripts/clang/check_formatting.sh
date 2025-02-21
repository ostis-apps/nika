#!/usr/bin/env bash
set -eo pipefail

cmake --preset release-with-tests-conan -DSC_CLANG_FORMAT_CODE=ON
cmake --build --preset release --target clangformat_check
