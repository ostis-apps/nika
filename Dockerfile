FROM ubuntu:noble as base
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow
# install dependencies
COPY ./scripts /tmp/scripts
COPY ./problem-solver/sc-machine/scripts /tmp/problem-solver/sc-machine/scripts
COPY ./problem-solver/sc-machine/requirements.txt /tmp/problem-solver/sc-machine/requirements.txt
RUN apt-get update && \
    apt-get install -y --no-install-recommends sudo ccache tini tzdata && \
    /tmp/scripts/install_problem_solver_deps.sh

FROM base as builder
ENV CCACHE_DIR=/ccache

RUN apt-get install -y --no-install-recommends git && \
    /tmp/scripts/install_problem_solver_deps.sh --dev

COPY . /nika
WORKDIR /nika/scripts
RUN --mount=type=cache,target=/ccache/ ./build_problem_solver.sh -r

FROM base as final
COPY --from=builder /nika/problem-solver/sc-machine/scripts /nika/problem-solver/sc-machine/scripts
COPY --from=builder /nika/problem-solver/sc-machine/requirements.txt /nika/problem-solver/sc-machine/requirements.txt

COPY --from=builder /nika/bin /nika/bin
COPY --from=builder /nika/scripts /nika/scripts
COPY --from=builder /nika/nika.ini /nika/nika.ini

WORKDIR /nika/scripts
ENTRYPOINT ["/usr/bin/tini", "--", "/nika/problem-solver/sc-machine/scripts/docker_entrypoint.sh"]
