#!/usr/bin/env bash
set -eo pipefail

kill_bg_process() {
    echo "Killing background processes with root PID $bg_process_pid."
    kill_tree $bg_process_pid
}

kill_tree() {
    local _pid=$1
    local _sig=${2:--TERM}
    # kill child processes
    for _child in $(pgrep -P $_pid); do
        kill_tree $_child $_sig
    done
    # kill the parent
    kill $_sig $_pid
}

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

"$APP_ROOT_PATH"/scripts/run_sc_machine.sh --repo_path "${APP_ROOT_PATH}"/kb.bin-tests --clear -p 8091 &
# gather launch script PID
bg_process_pid=$(jobs -p)
# kill sc-server on exit
trap kill_bg_process EXIT INT TERM

until python3 "${SC_MACHINE_PATH}/scripts/healthcheck.py" --port 8091 
do
    echo ...
    sleep 1
done
python3 -m unittest discover "$APP_ROOT_PATH"/problem-solver/py/
