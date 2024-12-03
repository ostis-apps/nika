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

## Rasa classifier installation

- Directory selection and creation
  ```sh
  mkdir rasa_model
  cd rasa_model
  ```
- Virtual environment creation and activation
  ```sh
  sudo apt install python3.8-venv
  python3.8 -m venv ./venv
  source ./venv/bin/activate
  ```
  
- Rasa module installation
  ```sh
  pip install rasa
  ```
  
- Rasa initialization 
  ```sh
  rasa init
  rasa train nlu
  rasa shell nlu
  ```

**Note**
```sh
rasa init
```

When running this command, 2 questions will be asked: choosing a folder for the classifier and offering to train the classifier immediately. There is no need to train the classifier, it should be done with the commands written above.

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
  ./scripts/run_sc_machine.sh

  # Terminal 2
  ./scripts/run_py_sc_server.sh
  
  # Terminal 3
  ./scripts/run_sc_web.sh
  
  # Terminal 4
  ./scripts/run_interface.sh
  
  # Terminal 5
  cd rasa_model
  rasa run --enable-api -m models/
  ```

In Terminal 5 after models/ should come name of the trained classifier model file. For example, nlu-20241129-200228-wooden-refund.tar.gz.

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
