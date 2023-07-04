#!/bin/bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"
"${PLATFORM_PATH}/scripts/run_sc_server.sh" "$@"
