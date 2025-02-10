#!/usr/bin/env bash
set -eo pipefail # stop script execution if any errors are encountered

# script help info
function usage() {
    cat <<USAGE

    Usage: 
        $0 build -b <path/to/sc-machine/binaries> -c <path/to/config.ini> [knowledge base sources path]
        $0 run -b <path/to/sc-machine/binaries> -c <path/to/config.ini> [sc-machine args]

    Options:
        build <PATH>:       Rebuilds KB from sources (provide absolute path to the source folder or repo.path file).
        run <args>:         Starts sc-machine. Arguments passed to this command will be redirected to sc-machine binary. You can set sc-server options from common config.

        Setting REBUILD_KB environment variable inside the container will trigger a KB rebuild. Setting custom starting point for sc-builder can be done using KB_PATH environment variable, "/knowledge-base" is used as a default KB_PATH.
        CONFIG_PATH and BINARY_PATH environment variables can provide the respective settings if the use of flags is undesirable.
        EXTENSIONS_PATH can be set to specify the path for extensions used by sc-machine.

USAGE
    exit 1
}

function build_kb() {
    if [ -e "$1" ];
    then
        "$BINARY_PATH"/sc-builder --clear -c "$CONFIG_PATH" -i "$@"
    elif [ -e "$KB_PATH" ];
    then
        echo "$KB_PATH is set as a KB path by the environment variable"
        "$BINARY_PATH"/sc-builder --clear -c "$CONFIG_PATH" -i "$KB_PATH"
    else
        echo "Invalid KB source path provided."
        exit 1
    fi
}

function start_machine {
    if [ -n "$REBUILD_KB" ] && [ "$REBUILD_KB" -eq 1 ];
    then
        # this expands to $KB_PATH if it's non-null and expands to "/knowledge-base" otherwise.
        build_kb "${KB_PATH:-"/knowledge-base"}"
    fi

    # if arguments were provided, use them instead of the default ones.
    if [ $# -eq 0 ];
    then
        # you should provide the config file path and host settings yourself in case you want to use custom options!
        echo "Using default arguments."
        "$BINARY_PATH"/sc-machine -c "$CONFIG_PATH" -e "$EXTENSIONS_PATH"
    else
        "$BINARY_PATH"/sc-machine "$@"
    fi
}

parse_options() {
    while getopts "b:c:h" opt; do
        case $opt in
            b) BINARY_PATH=$OPTARG ;;
            c) CONFIG_PATH=$OPTARG ;;
            h) usage ;;
            \?) echoerr "Invalid option -$OPTARG"; usage ;;
        esac
    done
    shift $((OPTIND - 1))
}

# parse script commands
case $1 in

# rebuild KB in case the build command was passed
build)
    shift 1;
    parse_options "$@"
    build_kb "$@"
    ;;

# launch sc-machine
run)
    shift 1;
    parse_options "$@"
    start_machine "$@"
    ;;

# show help
--help|help|-h)
    usage
    ;;

# All invalid commands will invoke usage page
*)
    usage
    ;;
esac
