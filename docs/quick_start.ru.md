# Быстрый старт

!!! Примечание
    В настоящее время NIKA не поддерживается нативно в Windows.

1. Клонируйте репозиторий NIKA:
   
    ```
    git clone https://github.com/NikitaZotov/nika
    cd nika
    git checkout feat/migrate_to_0.10.0
    git submodule update --init --recursive
    ```

2. Установите C++ решатель задач:

    ```
    ./scripts/install_cxx_problem_solver.sh
    ```

3. Соберите базу знаний:

    ```
    ./install/sc-machine/bin/sc-builder -i repo.path -o kb.bin --clear
    ```

4. Запустите C++ решатель задач:

    ```
    ./install/sc-machine/bin/sc-machine -s kb.bin -c nika.ini \
        -e "install/sc-machine/lib/extensions;install/scl-machine/lib/extensions;install/problem-solver/lib/extensions"
    ```

5. Установите и соберите sc-web. Откройте новый терминал и выполните:

    ```
    cd sc-web
    ./scripts/install_dependencies.sh
    npm run build
    ```

6. Запустите sc-web:
   
    ```
    source .venv/bin/activate && python3 server/app.py
    ```

7.  Установите зависимости Python решателя задач. Откройте новый терминал и выполните:

    ```
    cd ..
    python3 -m venv problem-solver/py/.venv
    source problem-solver/py/.venv/bin/activate
    pip3 install -r problem-solver/py/requirements.txt
    ```

8.  Запустите Python решатель задач:
    
    ```
    python3 problem-solver/py/server.py
    ```

9.  Установите и соберите React интерфейс. Откройте новый терминал и выполните:

    ```
    cd interface
    npm install
    npm run build
    ```

10. Запустите React интерфейс:

    ```
    npm run start
    ```
