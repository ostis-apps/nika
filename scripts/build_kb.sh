#!/bin/bash
if [[ -z ${APP_ROOT_PATH+1} ]];
then
  source set_vars.sh
fi
set -e -o pipefail
echo "${SC_MACHINE_PATH}"/scripts/build_kb.py 
python3 "${SC_MACHINE_PATH}"/scripts/build_kb.py -c "${APP_ROOT_PATH}/nika.ini" -b "${APP_ROOT_PATH}"/bin "${@:-"${APP_ROOT_PATH}/repo.path"}"
