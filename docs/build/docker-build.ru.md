# Сборка с помощью Docker

## Требования

Вам понадобится установленный и запущенный [Docker](https://docs.docker.com/) (с плагином Compose).

Рекомендуем использовать Docker Desktop на [macOS](https://docs.docker.com/desktop/install/mac-install/) / [Windows](https://docs.docker.com/desktop/install/windows-install/), а также использовать [Docker Server](https://docs.docker.com/engine/install/#server) дистрибутив для выбранного Вами Linux дистрибутива. Инструкции по установке можно найти по ссылкам сверху.

## Установка

```sh
git clone -c core.longpaths=true -c core.autocrlf=true https://github.com/ostis-apps/nika # для избежания проблем файловой системы Windows
cd nika
git submodule update --init --recursive
docker compose pull
```

## Сборка

  ```sh
  docker compose build
  ```

## 🚀 Запуск

  ```sh
  docker compose up --no-build
  ```

  Данная команда запустит 2 следующих web-интерфейса:
  
- sc-web - `localhost:8000`
- диалоговый web-интерфейс - `localhost:3033`

**Мы настроили нашу систему на пересборку БЗ (базы знаний) при каждом повторном запуске**. Если Вы разрабатываете какой-то конкретный фрагмент своей базы знаний, то можете изменить repo.path, чтобы убрать ненужные Вам папки.

**Если Вы не хотите пересобирать БЗ при повторном запуске**, то закомментируйте переменную окружения `REBUILD_KB` в `docker-compose.yml`.
Вы можете использовать `docker compose run --rm problem-solver build`, чтобы пересобрать БЗ вручную.

## Устранение неполадок

Проблемы, характерные для Windows:

- Сборка Docker-образов не запускается правильно и выдаёт что-то наподобие: `bash\r: No such file or directory`

    **Решение**: убедитесь, что Ваш Git репозиторий сконфигурирован на совместимость конца строк UNIX

    ```sh
    cd nika
    git config --local core.autocrlf true
    ```

- Git не может клонировать репозиторий или подмодули, ошибка выглядит так: `error: unable to create file ... (file too long)`

    **Решение**: убедитесь, что у Вашего Git репозитория свойство конфигурации `longpaths` включено:

    ```sh
    cd nika
    git config --local core.longpaths true
    ```

Общие проблемы:

- Docker-образы не могут быть собраны локально. Ошибка: `status: the --mount option requires BuildKit`

    **Решение**: Обратите внимание, что Вам локальная сборка нужна только для пользовательских образов, ситему можно запустить без сборки своих собственных образов. Используйте [Docker Docs BuildKit ссылку](https://docs.docker.com/go/buildkit), чтобы включить Docker BuildKit. **Если Вы используете Windows**, можете установить `$env:DOCKER_BUILDKIT = 1` во время сборки в PowerShell.

- Контейнер решателя задач `unhealthy`. Что делать?

    Похоже, что Ваш контейнер не запустился вовремя или не запустился корректно. Есть две главные причины для этого: не подходит `start_period` (в случае, если процесс запуска занимает слишком много времени на Вашем устройстве) или некорректный код на сервере. Для данных двух проблем, мы предлагаем два решения.

    **Решение 1**: Увеличение `start_period` в `docker-compose.yml` должно помочь.

    **Решение 2**: Проверьте [список ошибок](https://github.com/ostis-apps/nika/issues), если Вашу ошибку ещё не публиковали, создайте новый отчёт об ошибке!
