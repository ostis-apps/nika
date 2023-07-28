set(SOURCES
    "MessageReplyModule.cpp"
    "keynodes/MessageReplyKeynodes.cpp"
    "agent/MessageReplyAgent.cpp"
    "templates/MessageReplyTemplates.cpp"
)

set(HEADERS
    "MessageReplyModule.hpp"
    "keynodes/MessageReplyKeynodes.hpp"
    "agent/MessageReplyAgent.hpp"
    "templates/MessageReplyTemplates.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}
    ${SC_COMMON_MODULE_SRC}
)

add_library(messageReply SHARED ${SOURCES} ${HEADERS})
target_link_libraries(messageReply sc-memory sc-agents-common common)

sc_codegen_ex(messageReply ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(messageReply)
endif()
