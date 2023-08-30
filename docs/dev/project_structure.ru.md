# Project Structure

## kb
В данной папке хранятся фрагменты базы знаний. *(Например в файлах .scs)*

## problem-solver
В данной папке хранятся компоненты решателя задач в виде агентов и программ.

### Агенты на C++
Несколько советов:

- Храните свои модули с агентами c++ в *problem-solver/cxx*;

- После обновления кода на c++ вам необходимо пересобрать решатель задач. Просто запустите:
```
./scripts/build_problem_solver.sh
```
Для полной пересборки с удалением содержимого папок *bin* и *build* запустите:
```
./scripts/build_problem_solver.sh -f
```
Для сборки проекта в release mode запустите:
```
./scripts/build_problem_solver.sh -r
```
Для сборки тестов запустите:
```
./scripts/build_problem_solver.sh -t
```
Для полной пересборки с тестами и удалением содержимого папок *bin* и *build* запустите:
```
./scripts/build_problem_solver.sh -f -t
```

- Добавьте проверку деактивации действия с помощью функции *ActionUtils::isActionDeactivated()* из общего модуля. Идентификаторы действий для деактивации хранятся в *kb/non_subject_domain_concepts/action_deactivated.scs*. Пример:
```
#include "utils/ActionUtils.hpp"

sc_result MyModule::InitializeImpl()
{
  ScMemoryContext ctx(sc_access_lvl_make_min, "MyModule");
  if (ActionUtils::isActionDeactivated(&m_memoryCtx, Keynodes::action_of_my_agent))
  {
    SC_LOG_ERROR("My agent action is deactivated")
  }
  else
  {
    ...
  }
  return SC_RESULT_OK;
}
```

### Логирование
Вы можете изменить режим и уровень ведения логов в файле конфигурации nika.ini.

## sc-web
- После обновления нужно пересобрать sc-web. Для этого запустите:
```
./scripts/build_sc_web.sh
```

## interface
В данной папке можно хранить модули для интерфейса.

## scripts
В данной папке можно хранить скрипты.

### build_problem_solver.sh [-f, --full]
Сборка решателя задач Вашей программы. Используйте аргумент *-f* или *--full* для полной пересборки решателя задач с удалением содержимого папок *bin* и *build*.

### run_interface.sh
Запуск интерфейса.

### install_project.sh
Установка или обновление ostis-web-platform.

### install_subsystems.sh
Сборка решателя задач и базы знаний подсистем.

