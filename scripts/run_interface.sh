#!/usr/bin/env bash

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"
cd "${APP_ROOT_PATH}/interface" || exit 1
yarn && yarn start
