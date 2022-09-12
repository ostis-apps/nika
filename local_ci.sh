#!/bin/bash

check_black() {
  black --diff --line-length 120 problem-solver/py/services
}

check_pylint() {
  pylint problem-solver/py/services
}

argparser() {
  case $1 in
  "--black" | "-b")
    echo "checking with black"
    check_black
    ;;
  "--pylint" | "-p")
    echo "checking with pylint"
    check_pylint
    ;;
  "--all" | "-a")
    argparser "-b"
    argparser "-p"
    ;;
  esac
}

while [ "$1" != "" ]; do
	argparser "$1"
	shift
done
