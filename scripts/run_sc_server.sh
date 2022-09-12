#!/bin/bash
set -eo pipefail

source set_vars.sh

"$APP_ROOT_PATH"/bin/sc-server -c "$APP_ROOT_PATH"/nika.ini "$@"
