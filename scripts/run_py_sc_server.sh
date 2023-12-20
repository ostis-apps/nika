#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

python3 "$APP_ROOT_PATH"/problem-solver/py/server.py "$@"
