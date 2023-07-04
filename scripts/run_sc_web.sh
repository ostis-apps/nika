#!/bin/bash

set -eo pipefail

source set_vars.sh

cd "${APP_ROOT_PATH}"/sc-web/scripts
./run_scweb.sh
