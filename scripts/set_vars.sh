#!/bin/bash

APP_ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

export APP_ROOT_PATH="${APP_ROOT_PATH}"
export ROOT_CMAKE_PATH="${APP_ROOT_PATH}"
export PROBLEM_SOLVER_PATH="${APP_ROOT_PATH}/problem-solver"
export REPO_PATH_FILE="${APP_ROOT_PATH}/repo.path"
export SCRIPTS_PATH="${APP_ROOT_PATH}/scripts"
export KB_PATH="${APP_ROOT_PATH}/kb"

export SC_MACHINE_PATH="${PROBLEM_SOLVER_PATH}/sc-machine"
export SC_WEB_PATH="${APP_ROOT_PATH}/sc-web"

if [ -d "${SC_MACHINE_PATH}" ];
then
  source "${SC_MACHINE_PATH}/scripts/set_vars.sh"
fi

if [ -d "${SC_WEB_PATH}" ];
then
  source "${SC_WEB_PATH}/scripts/set_vars.sh"
fi
