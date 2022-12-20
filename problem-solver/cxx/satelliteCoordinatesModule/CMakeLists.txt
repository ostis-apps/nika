# установка псевдонима на путь к текущей директории
set(COORDINATES_AGENTS_SRC ${CMAKE_CURRENT_LIST_DIR})

# добавление всех исходников, оканчивающихся на ".cpp" и 
# ".hpp", в списки для цели сборки
file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

# включение исходников sc-памяти
include_directories(
        ${COORDINATES_AGENTS_SRC}
        ${SC_MEMORY_SRC}
        ${SC_KPM_SRC}
        ${SC_TOOLS_SRC}
        ${SC_COMMON_MODULE_SRC}
)

# создание библиотеки `satelliteCoordinatesAgentsModule` на базе собранных 
# исходников
add_library(satelliteCoordinatesAgentsModule SHARED ${SOURCES} ${HEADERS} agent/AddSatelliteCoordinatesAgent.cpp agent/AddSatelliteCoordinatesAgent.hpp keynodes/Keynodes.cpp keynodes/Keynodes.hpp manager/CoordinatesManager.cpp manager/CoordinatesManager.hpp agent/RemoveCoordinatesAgent.cpp agent/RemoveCoordinatesAgent.hpp agent/GetSatelliteFromMessageAgent.cpp agent/GetSatelliteFromMessageAgent.hpp)
# добавление зависимости `satelliteCoordinatesAgentsModule` библиотек от  
# `sc-memory` и `sc-agents-common`
add_dependencies(
        satelliteCoordinatesAgentsModule
        sc-memory
        sc-agents-common
)
# линковка зависимости `satelliteCoordinatesAgentsModule` библиотек от 
# `sc-memory` и `sc-agents-common`
target_link_libraries(
        satelliteCoordinatesAgentsModule
        sc-memory
        common
        sc-agents-common
)

# запуск генератора кода (необходим для создания тел 
# sc-агентов и ключевых элементов)
sc_codegen(satelliteCoordinatesAgentsModule ${COORDINATES_AGENTS_SRC})
