#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"
"${PLATFORM_PATH}/scripts/build_kb.sh" -i "${APP_ROOT_PATH}/repo.path"
