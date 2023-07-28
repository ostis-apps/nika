## Установка

```sh
git clone https://github.com/ostis-apps/nika
cd nika
git submodule update --init --recursive
./scripts/install_submodules.sh
./scripts/install_problem_solver_deps.sh
./scripts/install_interface_deps.sh
```

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
  ```
  Данные команды запустят 2 следующих web-интерфейса:

    - sc-web - `localhost:8000`
    - диалоговый web-интерфейс - `localhost:3033`

## Устранение неполадок
  - У Вас может появиться ошибка при запуске следующего скрипта из-за несовместимости версий Node-js:
    ```
    ./install_interface_deps.sh
    ```
    **Решение**: Убедитесь, что у Вас установлена необходимая версия Node-js. Попробуйте установить Node-js 16.

  - У Вас может возникнуть проблема с yarn. Например: `ERROR: There are no scenarios; must have at least one` при запуске следующего скрипта:
    ```
    ./run_interface.sh
    ```
    **Решение**: попробуйте переустановить yarn:
      ```
      sudo apt remove cmdtest
      sudo apt remove yarn
      curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
      echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
      sudo apt update
      sudo apt install yarn
      ```