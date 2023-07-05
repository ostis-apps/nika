## Installation

```sh
git clone https://github.com/ostis-apps/nika
cd nika
./scripts/prepare_platform.sh
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
