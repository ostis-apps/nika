#!/bin/bash
if [[ -z ${APP_ROOT_PATH+1} ]];
then
  source set_vars.sh
fi
set -e -o pipefail
"${APP_ROOT_PATH}"/bin/sc-builder -f --clear -c "${APP_ROOT_PATH}/nika.ini" -i "${@:-"${APP_ROOT_PATH}/repo.path"}" -o "${APP_ROOT_PATH}/kb.bin"
