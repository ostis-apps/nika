# Установка проекта нативно на Linux

## Установка

```sh
git clone https://github.com/ostis-apps/nika
cd nika
git submodule update --init --recursive
./scripts/install_submodules.sh
./scripts/install_problem_solver_deps.sh
./scripts/install_interface_deps.sh
./scripts/install_py_sc_server_deps.sh
```

## Установка Rasa классификатора

- Выбор и создание папки
  ```sh
  mkdir rasa_model
  cd rasa_model
  ```
- Создание и активация виртуального пространства
  ```sh
  sudo apt install python3.8-venv
  python3.8 -m venv ./venv
  source ./venv/bin/activate
  ```
  
- Установка Rasa модуля
  ```sh
  pip install rasa
  ```
  
- Инициализация Rasa 
  ```sh
  rasa init
  rasa train nlu
  rasa shell nlu
  ```

**Примечание**
```sh
rasa init
```

При запускее этой команды, будет задано 2 вопроса: выбор папки для классификатора и предложение сразу тренировать классификатор. Классификатор тренировать не надо, это стоит делать указанными командами выше. 

## Сборка

- Сборка решателя задач

  ```sh
  ./scripts/build_problem_solver.sh
  ```

- Сборка базы знаний

  ```sh
  ./scripts/build_kb.sh
  ```

- Сборка sc-web

  ```sh
  ./scripts/build_sc_web.sh
  ```

## 🚀 Запуск

```sh
# Терминал 1
./scripts/run_sc_server.sh

# Терминал 2
./scripts/run_sc_web.sh

# Терминал 3
./scripts/run_interface.sh

# Терминал 4
cd rasa_model
rasa run --enable-api -m models/
```

После models/ должен следовать файл заранее натренированной модели классификатора. Например, nlu-20241129-200228-wooden-refund.tar.gz.

  Данные команды запустят 2 следующих web-интерфейса:

- sc-web - `localhost:8000`
- диалоговый web-интерфейс - `localhost:3033`

## Устранение неполадок

- У Вас может появиться ошибка при запуске следующего скрипта из-за несовместимости версий Node-js:

  ```sh
  ./install_interface_deps.sh
  ```

  **Решение**: Убедитесь, что у Вас установлена необходимая версия Node-js. Попробуйте установить Node-js 16.

- У Вас может возникнуть проблема с yarn. Например: `ERROR: There are no scenarios; must have at least one` при запуске следующего скрипта:

  ```sh
  ./run_interface.sh
  ```

  **Решение**: попробуйте переустановить yarn:

    ```sh
    sudo apt remove cmdtest
    sudo apt remove yarn
    curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
    echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
    sudo apt update
    sudo apt install yarn
    ```
