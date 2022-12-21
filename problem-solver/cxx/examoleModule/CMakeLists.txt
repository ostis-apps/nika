set(SOURCES
        "agent/ExampleAgent.cpp"
        "keynodes/ExampleKeynodes.cpp"
        "ExampleModule.cpp"
        )

set(HEADERS
        "agent/ExampleAgent.hpp"
        "keynodes/ExampleKeynodes.hpp"
        "ExampleModule.hpp"
        )

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        ${SC_MEMORY_SRC}
        ${SC_KPM_SRC}
        ${SC_TOOLS_SRC}
        ${SC_COMMON_MODULE_SRC}
)

add_library(exampleModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(exampleModule sc-memory sc-agents-common common sc-builder-lib)

sc_codegen_ex(exampleModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)
