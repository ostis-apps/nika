set(DATE_TIME_AGENTS_SRC ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(
        ${DATE_TIME_AGENTS_SRC}
        ${SC_MEMORY_SRC}
        ${SC_KPM_SRC}
        ${SC_TOOLS_SRC}
        ${SC_COMMON_MODULE_SRC}
)

add_library(dateTimeAgentsModule SHARED ${SOURCES} ${HEADERS} agent/AddDateAndTimeAgent.cpp agent/AddDateAndTimeAgent.hpp keynodes/Keynodes.cpp keynodes/Keynodes.hpp manager/DateTimeManager.cpp manager/DateTimeManager.hpp agent/RemoveDateAndTimeAgent.cpp agent/RemoveDateAndTimeAgent.hpp agent/GetTimezoneFromMessageAgent.cpp agent/GetTimezoneFromMessageAgent.hpp)
add_dependencies(
        dateTimeAgentsModule
        sc-memory
        sc-agents-common
)
target_link_libraries(
        dateTimeAgentsModule
        sc-memory
        common
        sc-agents-common
)

sc_codegen(dateTimeAgentsModule ${DATE_TIME_AGENTS_SRC})
