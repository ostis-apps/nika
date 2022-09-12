#!/usr/bin/env bash
source set_vars.sh
packagelist=(
	python3-pip
	python3-setuptools
	build-essential
	cmake
	nlohmann-json3-dev
	libssl-dev
)
sudo apt-get install -y --no-install-recommends "${packagelist[@]}"
