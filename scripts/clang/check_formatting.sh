#!/usr/bin/env bash

set -eo pipefail

cmake -B build -DSC_CLANG_FORMAT_CODE=ON
cmake --build build --target clangformat_check
