# Quick Start

!!! Note
    Currently, using NIKA natively on Windows isn't supported.

1. Clone repository of the NIKA:
   
    ```sh
    git clone https://github.com/NikitaZotov/nika
    cd nika
    git checkout feat/migrate_to_0.10.0
    git submodule update --init --recursive
    ```

2. Install C++ problem solver:

    ```sh
    ./scripts/install_cxx_problem_solver.sh
    ```

3. Build knowledge base:

    ```sh
    ./install/sc-machine/bin/sc-builder -i repo.path -o kb.bin --clear
    ```

4. Run C++ problem solver:

    ```sh
    ./install/sc-machine/bin/sc-machine -s kb.bin -c nika.ini \
        -e "install/sc-machine/lib/extensions;install/scl-machine/lib/extensions;install/problem-solver/lib/extensions"
    ```

5. Install and build sc-web. Open new terminal and run:

    ```sh
    cd sc-web
    ./scripts/install_dependencies.sh
    npm run build
    ```

6. Run sc-web:
   
    ```sh
    source .venv/bin/activate && python3 server/app.py
    ```

7.  Install Python problem solver dependencies. Open new terminal and run in the root of the project:

    ```sh
    python3 -m venv problem-solver/py/.venv
    source problem-solver/py/.venv/bin/activate
    pip3 install -r problem-solver/py/requirements.txt
    ```

8.  Run Python problem-solver:
    
    ```sh
    python3 problem-solver/py/server.py
    ```

9.   Install and build React interface. Open new terminal and run:

    ```sh
    cd interface
    npm install
    npm run build
    ```

10. Run React interface:

    ```sh
    npm run start
    ```
