set(SOURCES
    "InterfaceModule.cpp"
    "keynodes/InterfaceKeynodes.cpp"
    "agent/InterfaceAgent.cpp"
)

set(HEADERS
    "InterfaceModule.hpp"
    "keynodes/InterfaceKeynodes.hpp"
    "agent/InterfaceAgent.hpp"
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

sc_codegen_ex(interface ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(interface)
endif()
