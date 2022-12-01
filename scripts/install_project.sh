#!/bin/bash
source set_vars.sh

do_build_kb=1
do_build_problem_solver=1
do_build_sc_web=1


build_kb()
{
	echo "Building KB"
	cd "${APP_ROOT_PATH}"/scripts
	echo "${APP_ROOT_PATH}"/scripts
	./build_kb.sh
}

build_problem_solver()
{
	cd "${APP_ROOT_PATH}"/scripts
	./install_deps_ubuntu.sh
	"${SC_MACHINE_PATH}/scripts/install_deps_ubuntu.sh" --dev
	./build_problem_solver.sh
}

build_sc_web() {
	cd "${APP_ROOT_PATH}"/sc-web || (echo "sc-web not downloaded." && exit 1)
	echo "${APP_ROOT_PATH}"/sc-web
	(cd scripts && ./install_deps_ubuntu.sh)
	pip3 install -r requirements.txt
	npm install
	npm run build
}

set -eo pipefail

cd "${APP_ROOT_PATH}"

git submodule update --init --recursive
build_problem_solver
build_sc_web

while [ "$1" != "" ]; do
	case $1 in
		"--no_build_kb" )
			do_build_kb=0
			;;
		"--no_build_problem_solver" )
			do_build_problem_solver=0
			do_build_kb=0
			;;
		"--no_build_sc_web" )
			do_build_sc_web=0
	esac
	shift
done

if [ $do_build_problem_solver == 1 ]; then
	build_problem_solver
fi

if [ $do_build_sc_web == 1 ]; then
	build_sc_web
fi

if [ $do_build_kb == 1 ]; then
	build_kb
fi

cd "${WORKING_PATH}"
