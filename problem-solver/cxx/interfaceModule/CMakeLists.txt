set(INFERENCE_MODULE_SRC "${SUBSYSTEMS_PATH}/scl-machine/problem-solver/cxx/inferenceModule")

set(SOURCES
    "InterfaceModule.cpp"
    "keynodes/InterfaceKeynodes.cpp"
    "agent/ChangeInterfaceColorAgent.cpp"
    "agent/CreateAnswerTemplate.cpp"
    "agent/TestAgent.cpp"
)

set(HEADERS
    "InterfaceModule.hpp"
    "keynodes/InterfaceKeynodes.hpp"
    "agent/ChangeInterfaceColorAgent.hpp"
    "agent/CreateAnswerTemplate.hpp"
    "agent/TestAgent.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}
    ${SC_COMMON_MODULE_SRC}
    ${INFERENCE_MODULE_SRC}
    ${DIALOG_CONTROL_MODULE_SRC}
)

add_library(interface SHARED ${SOURCES} ${HEADERS})
target_link_libraries(interface sc-memory sc-agents-common common dialogControlModule inferenceModule)

sc_codegen_ex(interface ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(interface)
endif()
