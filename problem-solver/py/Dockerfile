FROM python:3.11-slim AS base

USER root
WORKDIR /nika/problem-solver

COPY ./problem-solver/py/requirements.txt .
RUN --mount=type=cache,target=/root/.cache/pip pip3 install -r requirements.txt

COPY problem-solver/py/ ./problem-solver/py/

ENTRYPOINT [ "python3", "problem-solver/py/server.py" ]
