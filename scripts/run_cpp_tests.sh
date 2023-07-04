#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

"${BINARY_PATH}/sc-builder" -i "${SC_MACHINE_PATH}/sc-tools/sc-builder/tests/kb" -o "${BINARY_PATH}/sc-builder-test-repo" --clear -f

"${BINARY_PATH}/sc-builder" -i "${SC_MACHINE_PATH}/sc-tools/sc-server/tests/kb" -o "${BINARY_PATH}/sc-server-test-repo" --clear -f
(cd "${BUILD_PATH}" && ctest -C Debug -V -R 'starter' --timeout 30)
