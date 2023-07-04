#!/bin/bash

APP_ROOT_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && cd .. && pwd)

if [ "$1" == "-ci" ];
    then
      {
        echo SC_MACHINE_PATH="${APP_ROOT_PATH}/problem-solver/sc-machine"
        echo WORKING_PATH="$(pwd)"
        echo REPO_PATH_FILE="repo.path"
        echo SCRIPTS_PATH="${APP_ROOT_PATH}"/scripts
        echo KB_PATH="${APP_ROOT_PATH}"/kb
      } >> "$GITHUB_ENV"
        echo APP_ROOT_PATH="$(pwd)" >> "$GITHUB_ENV"
    else
        export SC_MACHINE_PATH="${APP_ROOT_PATH}/problem-solver/sc-machine"
        WORKING_PATH="$(pwd)"
        export WORKING_PATH
        export APP_ROOT_PATH
        export REPO_PATH_FILE="repo.path"
        export SCRIPTS_PATH="${APP_ROOT_PATH}"/scripts
        export KB_PATH="${APP_ROOT_PATH}"/kb
    fi
