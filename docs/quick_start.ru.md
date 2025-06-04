# Быстрый старт

!!! Примечание
    В настоящее время NIKA не поддерживается нативно в Windows.

1. Клонируйте репозиторий NIKA:
   
    ```
    git clone https://github.com/ostis-apps/nika
    cd nika
    git submodule update --init --recursive
    ```

2. Установите C++ решатель задач:

    ```
    ./scripts/install_cxx_problem_solver.sh
    ```

3. Соберите базу знаний:

    ```
    ./scripts/start.sh build_kb
    ```

4. Запустите C++ решатель задач:

    ```
    ./scripts/start.sh machine
    ```

5. Установите и соберите sc-web. Откройте новый терминал и выполните:

    ```
    cd sc-web
    ./scripts/install_dependencies.sh
    npm run build
    cd ..
    ```

6. Запустите sc-web:
   
    ```
    ./scripts/start.sh web
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
    ./scripts/start.sh py_server
    ```

9.  Установите и соберите React интерфейс. Откройте новый терминал и выполните:

    ```
    cd interface
    npm install
    npm run build
    ```

10. Запустите React интерфейс:

    ```
    ./scripts/start.sh interface
    ```
