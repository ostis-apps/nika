FROM python:3.11-slim as base

USER root
WORKDIR /nika/problem-solver

RUN apt-get update && apt-get install -y --no-install-recommends tini

COPY ./problem-solver/py/requirements.txt .
COPY scripts/install_py_sc_server_deps.sh scripts/set_vars.sh ./scripts/
RUN --mount=type=cache,target=/root/.cache/pip pip3 install -r requirements.txt

COPY problem-solver/py/ ./problem-solver/py/
COPY scripts/run_py_sc_server.sh ./scripts/

ENTRYPOINT [ "/usr/bin/tini", "--", "./scripts/run_py_sc_server.sh" ]
