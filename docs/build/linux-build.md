## Installation

```sh
git clone https://github.com/ostis-apps/nika
cd nika
./scripts/install_submodules.sh
./scripts/install_problem_solver_deps.sh
./scripts/install_interface_deps.sh
```

## Build
- Build knowledge base
  ```sh
  ./scripts/build_kb.sh
  ```

- Build problem solver
  ```sh
  ./scripts/build_problem_solver.sh
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
  ./scripts/run_sc_web.sh
  
  # Terminal 3
  ./scripts/run_interface.sh
  ```
  This commands will launch 2 Web UIs on your machine:
    - sc-web - `localhost:8000`
    - dialogue web UI - `localhost:3033`
