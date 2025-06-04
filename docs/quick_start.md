# Quick Start

!!! Note
    Currently, using NIKA natively on Windows isn't supported.

1. Clone repository of the NIKA:
   
    ```sh
    git clone https://github.com/ostis-apps/nika
    cd nika
    git submodule update --init --recursive
    ```

2. Install C++ problem solver:

    ```sh
    ./scripts/install_cxx_problem_solver.sh
    ```

3. Build knowledge base:

    ```sh
    ./scripts/start.sh build_kb
    ```

4. Run C++ problem solver:

    ```sh
    ./scripts/start.sh machine
    ```

5. Install and build sc-web. Open new terminal and run:

    ```sh
    cd sc-web
    ./scripts/install_dependencies.sh
    npm run build
    cd ..
    ```

6. Run sc-web:
   
    ```sh
    ./scripts/start.sh web
    ```

7.  Install Python problem solver dependencies. Open new terminal and run in the root of the project:

    ```sh
    python3 -m venv problem-solver/py/.venv
    source problem-solver/py/.venv/bin/activate
    pip3 install -r problem-solver/py/requirements.txt
    ```

8.  Run Python problem-solver:
    
    ```sh
    ./scripts/start.sh py_server
    ```

9.   Install and build React interface. Open new terminal and run:

    ```sh
    cd interface
    npm install
    npm run build
    ```

10. Run React interface:

    ```sh
    ./scripts/start.sh interface
    ```
