#!/usr/bin/env bash
set -eo pipefail

install_problem_solver()
{
  "${APP_ROOT_PATH}/scripts/install_deps_ubuntu.sh"
  "${SC_MACHINE_PATH}/scripts/install_deps_ubuntu.sh" --dev
}

install_sc_web()
{
  "${SC_WEB_PATH}/scripts/install_deps_ubuntu.sh"
  "${APP_ROOT_PATH}/build_sc_web.sh"
}

git submodule update --init --recursive
install_problem_solver
install_sc_web
