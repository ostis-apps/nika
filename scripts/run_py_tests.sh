#!/usr/bin/env bash
set -eo pipefail

kill_bg_process() {
    echo "Killing background process with PID $bg_process_pid."
    kill $bg_process_pid
}

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

"$APP_ROOT_PATH"/scripts/run_sc_server.sh --repo_path "${APP_ROOT_PATH}"/kb.bin-tests --clear &
# gather sc-server PID
bg_process_pid=$!
# kill sc-server on exit
trap kill_bg_process EXIT INT TERM

until python3 "${SC_MACHINE_PATH}/scripts/healthcheck.py"
do
    echo ...
    sleep 1
done
python3 -m unittest discover "$APP_ROOT_PATH"/problem-solver/py/
