## Requirements
You will need [Docker](https://docs.docker.com/) (with Compose plugin) installed and running on your machine.

We recommend using Docker Desktop on [macOS](https://docs.docker.com/desktop/install/mac-install/) / [Windows](https://docs.docker.com/desktop/install/windows-install/) and using [Docker Server](https://docs.docker.com/engine/install/#server) distribution for your Linux distribution of choice. Use installation instructions provided in the links above.
## Installation

```sh
git clone -c core.longpaths=true -c core.autocrlf=true https://github.com/ostis-apps/nika
cd nika
git submodule update --init --recursive
docker compose pull
```

## Build
  ```sh
  docker compose build
  ```

## 🚀 Run
  ```sh
  docker compose up --no-build
  ```
  This command will launch 2 Web UIs on your machine:
    - sc-web - `localhost:8000`
    - dialogue web UI - `localhost:3033`

We've set our system to rebuild KB on each restart. If you're debugging some specific subset of your knowledge base you may want to change repo.path to exclude the folders you don't need.

If you do not want to rebuild KB on relaunch, you can comment out the `REBUILD_KB` environment variable in `docker-compose.yml`.
You can use `docker compose run machine build` to rebuild KB manually.
