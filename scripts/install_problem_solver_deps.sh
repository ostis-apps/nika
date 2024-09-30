#!/usr/bin/env bash
set -eo pipefail

source "$(cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd)/set_vars.sh"

packagelist=(
	python3-pip
	python3-setuptools
	build-essential
	cmake
	nlohmann-json3-dev
	libssl-dev
	file
	libcurl4-openssl-dev
)
sudo apt-get install -y --no-install-recommends "${packagelist[@]}"

"${SC_MACHINE_PATH}/scripts/install_deps_ubuntu.sh" --dev
