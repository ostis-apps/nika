#!/usr/bin/env bash
set -eo pipefail

SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT_PATH="$(cd "$SCRIPT_PATH/.." && pwd)"

SC_MACHINE_PATH="$PROJECT_ROOT_PATH"/install/sc-machine
SCL_MACHINE_PATH="$PROJECT_ROOT_PATH"/install/scl-machine
PROBLEM_SOLVER_PATH="$PROJECT_ROOT_PATH"/install/problem-solver
LIB_PATH="$SCL_MACHINE_PATH/lib:$SC_MACHINE_PATH/lib:$PROBLEM_SOLVER_PATH/lib"

OS_TYPE="$(uname -s)"

case "$1" in
  build_kb)
    "$PROJECT_ROOT_PATH"/install/sc-machine/bin/sc-builder -i $PROJECT_ROOT_PATH/repo.path -o $PROJECT_ROOT_PATH/kb.bin --clear
    ;;
  machine)
    if [ "$OS_TYPE" = "Darwin" ]; then
      DYLD_LIBRARY_PATH="$LIB_PATH" \
      $SC_MACHINE_PATH/bin/sc-machine -s $PROJECT_ROOT_PATH/kb.bin \
        -e "$SC_MACHINE_PATH/lib/extensions;$SCL_MACHINE_PATH/lib/extensions;$PROBLEM_SOLVER_PATH/lib/extensions" -c $PROJECT_ROOT_PATH/nika.ini
    elif [ "$OS_TYPE" = "Linux" ]; then
      LD_LIBRARY_PATH="$LIB_PATH" \
      $SC_MACHINE_PATH/bin/sc-machine -s $PROJECT_ROOT_PATH/kb.bin \
        -e "$SC_MACHINE_PATH/lib/extensions;$SCL_MACHINE_PATH/lib/extensions;$PROBLEM_SOLVER_PATH/lib/extensions" -c $PROJECT_ROOT_PATH/nika.ini
    else
      echo "Unsupported OS: $OS_TYPE"
      exit 1
    fi
    ;;
  py_server)
    source $PROJECT_ROOT_PATH/problem-solver/py/.venv/bin/activate
    python3 $PROJECT_ROOT_PATH/problem-solver/py/server.py
    ;;
  web)
    cd $PROJECT_ROOT_PATH/sc-web || exit 1
    source .venv/bin/activate
    python3 server/app.py
    ;;
  interface)
    cd $PROJECT_ROOT_PATH/interface
    npm run start
    ;;
  *)
    echo "Usage: $0 {build_kb|machine|py_server|web|interface}"  
    exit 1
    ;;
esac
