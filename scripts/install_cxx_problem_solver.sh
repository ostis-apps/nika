#!/usr/bin/env bash
set -eo pipefail

# Constants
SC_MACHINE_VERSION="0.10.0"
SC_MACHINE_DESTINATION_DIR="install/sc-machine"

SCL_MACHINE_VERSION="0.3.0"
SCL_MACHINE_DESTINATION_DIR="install/scl-machine"

NIKA_VERSION="0.1.0"
NIKA_DESTINATION_DIR="install/problem-solver"

get_archive_name() {
    local os_name=$(uname -s)
    case "$os_name" in
        Linux)
            echo "$1-$2-Linux.tar.gz"
            ;;
        Darwin)
            echo "$1-$2-Darwin.tar.gz"
            ;;
        *)
            echo "Unsupported operating system: $os_name"
            exit 1
            ;;
    esac
}

download_archive() {
    local url="$1"
    echo "Downloading ${url}..."
    if ! curl -LO "${url}"; then
        echo "Error downloading ${url}"
        exit 1
    fi
}

extract_archive() {
    local archive="$1"
    local destination_dir="$2"
    echo "Creating directory ${destination_dir} and extracting files..."
    mkdir -p "${destination_dir}" && tar -xvzf "${archive}" -C "${destination_dir}" --strip-components 1
}

cleanup() {
    local archive="$1"
    local destination_dir="$2"
    echo "Cleaning up..."
    rm -f "${archive}"
    rm -rf "${destination_dir}/include"
}

SC_MACHINE_ARCHIVE=$(get_archive_name "sc-machine" "${SC_MACHINE_VERSION}")
SC_MACHINE_URL="https://github.com/ostis-ai/sc-machine/releases/download/${SC_MACHINE_VERSION}/${SC_MACHINE_ARCHIVE}"

download_archive "${SC_MACHINE_URL}"
extract_archive "${SC_MACHINE_ARCHIVE}" "${SC_MACHINE_DESTINATION_DIR}"
cleanup "${SC_MACHINE_ARCHIVE}" "${SC_MACHINE_DESTINATION_DIR}"

SCL_MACHINE_ARCHIVE=$(get_archive_name "scl-machine" "${SCL_MACHINE_VERSION}")
SCL_MACHINE_URL="https://github.com/ostis-ai/scl-machine/releases/download/${SCL_MACHINE_VERSION}/${SCL_MACHINE_ARCHIVE}"

download_archive "${SCL_MACHINE_URL}"
extract_archive "${SCL_MACHINE_ARCHIVE}" "${SCL_MACHINE_DESTINATION_DIR}"
cleanup "${SCL_MACHINE_ARCHIVE}" "${SCL_MACHINE_DESTINATION_DIR}"

NIKA_ARCHIVE=$(get_archive_name "nika" "${NIKA_VERSION}")
NIKA_URL="https://github.com/ostis-apps/nika/releases/download/${NIKA_VERSION}/${NIKA_ARCHIVE}"

download_archive "${NIKA_URL}"
extract_archive "${NIKA_ARCHIVE}" "${NIKA_DESTINATION_DIR}"
cleanup "${NIKA_ARCHIVE}" "${NIKA_DESTINATION_DIR}"

echo "Installation of NIKA complete!"
