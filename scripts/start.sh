#!/usr/bin/env bash
set -eo pipefail

SCRIPT_PATH="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT_PATH="$(cd "$SCRIPT_PATH/.." && pwd)"

SC_MACHINE_PATH="$PROJECT_ROOT_PATH"/install/sc-machine
SCL_MACHINE_PATH="$PROJECT_ROOT_PATH"/install/scl-machine
PROBLEM_SOLVER_PATH="$PROJECT_ROOT_PATH"/install/problem-solver
LD_LIBRARY_PATH="$SCL_MACHINE_PATH/lib:$SC_MACHINE_PATH/lib:$LD_LIBRARY_PATH"

case "$1" in
  build_kb)
    "$PROJECT_ROOT_PATH"/install/sc-machine/bin/sc-builder -i repo.path -o kb.bin --clear
    ;;
  machine)
    LD_LIBRARY_PATH="$LD_LIBRARY_PATH" \
      $SC_MACHINE_PATH/bin/sc-machine -s kb.bin \
      -e "$SC_MACHINE_PATH/lib/extensions;$SCL_MACHINE_PATH/lib/extensions;$PROBLEM_SOLVER_PATH/lib/extensions;" -c nika.ini
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
