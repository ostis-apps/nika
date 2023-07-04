FROM ubuntu:focal as base
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Moscow
# install dependencies
COPY ./scripts/install_deps_ubuntu.sh /tmp/install_deps_ubuntu.sh
RUN apt-get update && \
    apt-get install -y --no-install-recommends sudo ccache tzdata git && \ 
    /tmp/install_deps_ubuntu.sh

FROM base as builder
ENV CCACHE_DIR=/ccache

WORKDIR /nika
COPY . .
RUN --mount=type=cache,target=/ccache/ cd /nika/scripts && ./prepare_platform.sh

FROM base as final
COPY --from=builder /nika/problem-solver/sc-machine/scripts /nika/problem-solver/sc-machine/scripts
COPY --from=builder /nika/problem-solver/sc-machine/requirements.txt /nika/problem-solver/sc-machine/requirements.txt
RUN /nika/problem-solver/sc-machine/scripts/install_deps_ubuntu.sh && apt-get install -y --no-install-recommends tini

COPY --from=builder /nika/bin /nika/bin
COPY --from=builder /nika/scripts /nika/scripts
COPY --from=builder /nika/nika.ini /nika/nika.ini

WORKDIR /nika/scripts
ENTRYPOINT ["tini", "--", "/nika/problem-solver/sc-machine/scripts/docker_entrypoint.sh"]
