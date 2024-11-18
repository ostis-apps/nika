#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

(cd "${BUILD_PATH}" && ctest -C Debug -V -R 'starter' --timeout 30)
