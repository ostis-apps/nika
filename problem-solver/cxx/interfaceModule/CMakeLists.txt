set(SOURCES
    "InterfaceModule.cpp"
    "agent/ChangeInterfaceColorAgent.cpp"
)

set(HEADERS
    "InterfaceModule.hpp"
    "keynodes/InterfaceKeynodes.hpp"
    "agent/ChangeInterfaceColorAgent.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}
    ${SC_COMMON_MODULE_SRC}
)

add_library(interface SHARED ${SOURCES} ${HEADERS})
target_link_libraries(interface sc-memory sc-agents-common common)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(interface)
endif()
