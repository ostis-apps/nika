#!/bin/bash
set -e # stop script execution if any errors are encountered

# the path to the script directory. Used for relative paths
SCRIPTS_PATH=$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)

# script help info
function usage() {
    cat <<USAGE
    Usage: $0 [--rebuild_kb <PATH>] [sc-server args]
    Options:
        build <PATH>:       rebuilds KB from sources (provide absolute path to the source folder or repo.path file)
        serve <args>:       Starts sc-server. Arguments passed to this command will be redirected to sc-server binary. If no arguments were given, uses "-c /nika/nika.ini -h 0.0.0.0" as default sc-server arguments. Add host argument yourself if you are planning to use custom arguments.
        Setting REBUILD_KB environment variable inside the container will trigger a KB rebuild. Setting custom starting point for build_kb.py can be done using KB_PATH environment variable, "/kb" is used as a default KB_PATH.
USAGE
    "$SCRIPTS_PATH"/../bin/sc-server --help
    exit 1
}

function rebuild_kb() {
    if [ -e "$1" ]; then
        "$SCRIPTS_PATH"/build_kb.sh "$1"
    elif [ -n "$KB_PATH" ]; then
        echo "$KB_PATH is set as a KB path by the environment variable"
        "$SCRIPTS_PATH"/build_kb.sh "$KB_PATH"
    else
        echo "Invalid KB source path provided."
        exit 1
    fi
}

function start_server() {
if [ "$REBUILD_KB" -eq 1 ]; then
    # this expands to $KB_PATH if it's non-null and expands to "/kb" otherwise.
    rebuild_kb "${KB_PATH:-"/kb"}"
fi

# if arguments were provided, use them instead of the default ones.
if [ -e "$1" ]; then
    # you should provide the config file path and host settings yourself in case you want to use custom options!
    /nika/scripts/run_sc_server.sh "$@"
else
    echo "Using default arguments."
    /nika/scripts/run_sc_server.sh -h 0.0.0.0
fi
}


# parse script commands
case $1 in

    # rebuild KB in case the build command was passed
    build)
        rebuild_kb "$2"
        ;;

    #launch sc-server
    serve)
        start_server "${@:2}"
        ;;

    # show help
    --help)
        usage
        ;;
    help)
        usage
        ;;
    -h)
        usage
        ;;

    # All invalid commands will invoke usage page
    *)
        usage
        ;;
esac
