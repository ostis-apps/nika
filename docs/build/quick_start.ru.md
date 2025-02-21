# Быстрый старт для разработчиков

Этот гид предоставляет краткую информацию для разработчиков, чтобы быстро начать работу с NIKA. Вы всегда можете узнать больше о [системе сборки](build_system.md) NIKA.

## Установка подмодулей NIKA

Чтобы клонировать репозиторий, выполните:

```sh
git clone https://github.com/NikitaZotov/nika
cd nika
git checkout feat/migrate_to_0.10.0
git submodule update --init --recursive
```

## Проверка CMake

Сначала установите pipx, используя [**руководство по установке pipx**](https://pipx.pypa.io/stable/installation/), если он еще не установлен.

Убедитесь, что используете **CMake версии 3.24** или новее. Проверьте версию с помощью:

```sh
cmake --version
```

Чтобы обновить CMake, выполните:
  
```sh
# Установите cmake с помощью pipx, если он еще не установлен
pipx install cmake
pipx ensurepath
# Перезапустите shell после установки
exec $SHELL
```

Установите генератор Ninja для CMake, чтобы использовать пресеты CMake:

```sh
# Установите ninja с помощью pipx, если он еще не установлен
pipx install ninja
pipx ensurepath
# Перезапустите shell после установки
exec $SHELL
```

## Разработка C++ решателя задач NIKA с Conan

### Установка Conan

Установите Conan, чтобы собирать зависимости C++ решателя задач:

```sh
# Установите conan с помощью pipx, если он еще не установлен
pipx install conan
pipx ensurepath
# Перезапустите shell после установки
exec $SHELL
```

### Использование C++ решателя задач в режиме Debug

#### Установка зависимостей с Conan

C++ решатель задач NIKA является расширением для sc-machine, поэтому sc-machine - основная зависимость. Для установки выполните в корневой директории проекта:

```sh
conan install . -s build_type=Debug --build=missing
```

#### Сборка C++ решателя задач в режиме Debug

Для сборки в режиме отладки с использованием зависимостей Conan выполните:

```sh
# тип сборки debug
cmake --preset debug-conan
cmake --build --preset debug
```

!!! Примечание
    По умолчанию пресет `debug` включает тесты для C++ решателя задач.

#### Запуск тестов C++ решателя задач в режиме Debug

Затем перейдите в `build/Debug` и запустите тесты с помощью `ctest`:

```sh
cd build/Debug
ctest -V
```

Вы также можете проверить форматирование кода. Подробнее смотрите на странице [флагов CMake](cmake_flags.md).

### Использование C++ решателя задач в режиме Release

#### Установка зависимостей с Conan

Для установки выполните в корневой директории проекта:

```sh
conan install . -s build_type=Release --build=missing
```

#### Сборка C++ решателя задач в режиме Release

Для сборки без тестов выполните:

```sh
cmake --preset release-conan
cmake --build --preset release
```

Для сборки с тестами выполните:

```sh
cmake --preset release-with-tests-conan
cmake --build --preset release
```

#### Запуск тестов C++ решателя задач в режиме Release

После этого выполните:

```sh
cd build/Release
ctest -V
```

Вы также можете проверить форматирование кода. Подробнее смотрите на странице [флагов CMake](cmake_flags.md).

## Разработка sc-web интерфейса

### Установка зависимостей с npm

Перейдите в директорию `sc-web` и выполните:

```sh
cd sc-web
./scripts/install_dependencies.sh
```

### Сборка sc-web интерфейса

Для сборки выполните:

```sh
npm run build
```

### Запуск sc-web интерфейса

Для запуска выполните:

```sh
source .venv/bin/activate && python3 server/app.py
```

## Разработка Python решателя задач NIKA

### Установка зависимостей с pip3

Для установки зависимостей выполните:

```sh
python3 -m venv problem-solver/py/.venv
source problem-solver/py/.venv/bin/activate
pip3 install -r problem-solver/py/requirements.txt
```

### Запуск тестов Python решателя задач

Для запуска Python решателя задач требуется работающий C++ решатель задач. Запустите его:

```sh
./path/to/sc-machine/binary -s kb.bin -c nika.ini \
    -e "path/to/nika/lib/extensions;path/to/sc-machine/lib/extensions;path/to/scl-machine/lib/extensions"
```

Затем в новом терминале выполните:

```sh
source problem-solver/py/.venv/bin/activate && \
python3 -m unittest discover problem-solver/py
```

### Запуск Python решателя задач

Запустите C++ решатель задач:

```sh
./path/to/sc-machine/binary -s kb.bin -c nika.ini \
    -e "path/to/nika/lib/extensions;path/to/sc-machine/lib/extensions;path/to/scl-machine/lib/extensions"
```

Затем в новом терминале выполните:

```sh
source problem-solver/py/.venv/bin/activate && \
python3 problem-solver/py/server.py
```

## Разработка интерфейса NIKA

### Установка зависимостей с npm

Перейдите в директорию `interface` и выполните:

```sh
cd interface
npm install
```

### Сборка интерфейса

Для сборки выполните:

```sh
npm run build
```

### Запуск интерфейса

Для запуска выполните:

```sh
npm run start
```

## Запуск NIKA

Перед запуском NIKA [извлеките sc-machine из GitHub Releases](https://ostis-ai.github.io/sc-machine/quick_start/#github-releases) или [соберите его](https://ostis-ai.github.io/sc-machine/build/quick_start/).

Сначала соберите базу знаний:

```sh
./path/to/sc-builder/binary -i repo.path -o kb.bin --clear
```

Затем запустите C++ решатель задач:

```sh
./path/to/sc-machine/binary -s kb.bin -c nika.ini \
    -e "path/to/nika/lib/extensions;path/to/sc-machine/lib/extensions;path/to/scl-machine/lib/extensions"
```

Запустите sc-web интерфейс в новом терминале:

```sh
cd sc-web
source .venv/bin/activate && python3 server/app.py
```

Запустите Python решатель задач в новом терминале:

```sh
source problem-solver/py/.venv/bin/activate && \
python3 problem-solver/py/server.py
```

И запустите интерфейс в новом терминале:

```sh
cd interface
npm run start
```

### Участие в разработке

Чтобы внести изменения в проект, создайте [Pull Request](https://github.com/ostis-ai/nika/blob/main/docs/CONTRIBUTING.md).
