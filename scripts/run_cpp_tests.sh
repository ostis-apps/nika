#!/usr/bin/env bash
set -eo pipefail
source set_vars.sh

"$APP_ROOT_PATH"/bin/sc-builder -i "$SC_MACHINE_PATH"/sc-tools/sc-builder/tests/kb -o "$APP_ROOT_PATH"/bin/sc-builder-test-repo --clear -f
"$APP_ROOT_PATH"/bin/sc-builder -i "$SC_MACHINE_PATH"/sc-tools/sc-server/tests/kb -o "$APP_ROOT_PATH"/bin/sc-server-test-repo --clear -f

(cd "$APP_ROOT_PATH"/build && ctest -C Debug -V)
