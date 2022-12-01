#!/usr/bin/env bash
if [[ -z ${APP_ROOT_PATH+1} ]];
then
  source set_vars.sh
fi

cd "${APP_ROOT_PATH}"/interface || exit 1
yarn && yarn start
