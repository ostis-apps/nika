FROM ubuntu:noble AS base

ENV CCACHE_DIR=/ccache
USER root

COPY scripts /nika/scripts
COPY conanfile.py /nika/conanfile.py
COPY CMakePresets.json /nika/CMakePresets.json
COPY CMakeLists.txt /nika/CMakeLists.txt
COPY requirements.txt /nika/requirements.txt

# tini is an init system to forward interrupt signals properly
RUN apt update && apt install -y --no-install-recommends sudo tini curl ccache python3 python3-pip pipx cmake build-essential ninja-build

# Install Conan
RUN pipx install conan && \
    pipx ensurepath

FROM base AS devdeps
WORKDIR /nika

SHELL ["/bin/bash", "-c"]
RUN python3 -m venv /nika/.venv && \
    source /nika/.venv/bin/activate && \
    pip3 install -r /nika/requirements.txt

ENV PATH="/root/.local/bin:$PATH"
RUN conan remote add ostis-ai https://conan.ostis.net/artifactory/api/conan/ostis-ai-library && \
    conan profile detect && \
    conan install . --build=missing

# Install sc-machine binaries
RUN curl -LO https://github.com/ostis-ai/sc-machine/releases/download/0.10.0/sc-machine-0.10.0-Linux.tar.gz && \
    mkdir -p install/sc-machine && tar -xvzf sc-machine-0.10.0-Linux.tar.gz -C install/sc-machine --strip-components 1 && \
    rm -rf sc-machine-0.10.0-Linux.tar.gz && rm -rf install/sc-machine/include

# Install scl-machine libraries
RUN curl -LO https://github.com/NikitaZotov/scl-machine/releases/download/0.3.0/scl-machine-0.3.0-Linux.tar.gz && \
    mkdir -p install/scl-machine && tar -xvzf scl-machine-0.3.0-Linux.tar.gz -C install/scl-machine --strip-components 1 && \
    rm -rf scl-machine-0.3.0-Linux.tar.gz && rm -rf install/scl-machine/include

FROM devdeps AS devcontainer
RUN apt install -y --no-install-recommends cppcheck valgrind gdb bash-completion ninja-build curl
ENTRYPOINT ["/bin/bash"]

FROM devdeps AS builder
COPY . .
RUN --mount=type=cache,target=/ccache/ cmake --preset release-conan && cmake --build --preset release

# Gathering all artifacts together
FROM devdeps AS final

COPY --from=builder /nika/scripts /nika/scripts
COPY --from=builder /nika/install /nika/install
COPY --from=builder /nika/build/Release /nika/build/Release
COPY --from=builder /nika/.venv /nika/.venv

WORKDIR /nika

EXPOSE 8090

ENTRYPOINT ["/usr/bin/tini", "--", "/nika/scripts/docker_entrypoint.sh"]
