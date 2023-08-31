# Native installation on Linux

## Installation

```sh
git clone https://github.com/ostis-apps/nika
cd nika
git submodule update --init --recursive
./scripts/install_submodules.sh
./scripts/install_problem_solver_deps.sh
./scripts/install_interface_deps.sh
./scripts/install_py_sc_server_deps.sh
```

## Build

- Build problem solver

  ```sh
  ./scripts/build_problem_solver.sh
  ```

- Build knowledge base

  ```sh
  ./scripts/build_kb.sh
  ```

- Build sc-web

  ```sh
  ./scripts/build_sc_web.sh
  ```

## 🚀 Run

  ```sh
  # Terminal 1
  ./scripts/run_sc_server.sh

  # Terminal 2
  ./scripts/run_py_sc_server.sh
  
  # Terminal 3
  ./scripts/run_sc_web.sh
  
  # Terminal 4
  ./scripts/run_interface.sh
  ```

  This commands will launch 2 Web UIs on your machine:

- sc-web - `localhost:8000`
- dialogue web UI - `localhost:3033`

## Troubleshooting

- You can have a problem while running the next script:

  ```sh
  ./install_interface_deps.sh
  ```
  
  **Solution**: Please make sure that you have the nessesary version of Node-js. Try to download Node-js 16.

- You can have a problem with `yarn` which looks like `ERROR: There are no scenarios; must have at least one` while running the next script:

  ```sh
  ./run_interface.sh
  ```

  **Solution**: you can try to reinstall it:

    ```sh
    sudo apt remove cmdtest
    sudo apt remove yarn
    curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
    echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
    sudo apt update
    sudo apt install yarn
    ```
