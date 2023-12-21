set(SOURCES
    "TestModule.cpp"
    "keynodes/TestKeynodes.cpp"
    "agent/TestAgent.cpp"
    "agent/AuthorizationAgent.cpp"
    "agent/RegistrationAgent.cpp"
    "agent/TestingAgent.cpp"
)

set(HEADERS
    "TestModule.hpp"
    "keynodes/TestKeynodes.hpp"
    "agent/TestAgent.hpp"
    "agent/AuthorizationAgent.hpp"
    "agent/RegistrationAgent.hpp"
    "agent/TestingAgent.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}
    ${SC_COMMON_MODULE_SRC}
    ${DIALOG_CONTROL_MODULE_SRC}
)

add_library(testModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(testModule sc-memory sc-agents-common common dialogControlModule)

sc_codegen_ex(testModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(testModule)
endif()
