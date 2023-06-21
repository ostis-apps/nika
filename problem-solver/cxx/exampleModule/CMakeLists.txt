# установка псевдонима на путь к текущей директории
set(SC_EXAMPLE_AGENTS_SRC ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# добавление всех исходников, оканчивающихся на ".cpp" и 
# ".hpp", в списки для цели сборки
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

# включение исходников sc-памяти
include_directories(
${SC_EXAMPLE_AGENTS_SRC}
${SC_MEMORY_SRC}
${SC_KPM_SRC}
${DIALOG_CONTROL_MODULE_SRC}
)

# создание библиотеки `exampleModule` на базе собранных 
# исходников
add_library(exampleModule SHARED ${SOURCES} ${HEADERS})
# добавление зависимости `exampleModule` библиотек от  
# `sc-memory` и `sc-agents-common`
add_dependencies(
exampleModule
sc-memory
sc-agents-common
dialogControlModule
)
# линковка зависимости `exampleModule` библиотек от 
# `sc-memory` и `sc-agents-common`
target_link_libraries(
exampleModule
sc-memory
sc-agents-common
dialogControlModule
)

# запуск генератора кода (необходим для создания тел 
# sc-агентов и ключевых элементов)
sc_codegen_ex(exampleModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)