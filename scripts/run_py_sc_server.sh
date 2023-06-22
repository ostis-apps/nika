#!/bin/bash
set -eo pipefail

source set_vars.sh

python3 "$APP_ROOT_PATH"/problem-solver/py/server.py