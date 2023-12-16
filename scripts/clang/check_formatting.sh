#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/../set_vars.sh"

cmake -B "${BUILD_PATH}" -DSC_CLANG_FORMAT_CODE=ON "${APP_ROOT_PATH}"
cmake --build "${BUILD_PATH}" --target clangformat_check "${APP_ROOT_PATH}"
