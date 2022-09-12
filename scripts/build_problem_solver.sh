#!/bin/bash
echo -en '\E[47;31m'"\033[1mBuild sc-machine\033[0m\n"
tput sgr0
set -eo pipefail

source set_vars.sh


if [ "$1" == "--full" ] || [ "$1" == "-f" ];
	then
		rm -rf "${APP_ROOT_PATH}"/build
		rm -rf "${APP_ROOT_PATH:?}"/bin
		find "${APP_ROOT_PATH}"/problem-solver/cxx/ -type d -name generated -exec rm -rf {} +
fi

# check last argument
if [ "${!#}" == "--tests" ] || [ "${!#}" == "-t" ];
	then
		cmake -B "${APP_ROOT_PATH}"/build "${APP_ROOT_PATH}" -DSC_AUTO_TEST=ON -DSC_BUILD_TESTS=ON
	else
		cmake -B "${APP_ROOT_PATH}"/build "${APP_ROOT_PATH}"
fi

cmake --build "${APP_ROOT_PATH}"/build -j$(nproc)
