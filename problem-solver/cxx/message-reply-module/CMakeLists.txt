set(SOURCES
    "MessageReplyModule.cpp"
    "agent/MessageReplyAgent.cpp"
    "generator/MessageHistoryGenerator.cpp"
    "templates/MessageReplyTemplates.cpp"
)

set(HEADERS
    "MessageReplyModule.hpp"
    "keynodes/MessageReplyKeynodes.hpp"
    "agent/MessageReplyAgent.hpp"
    "generator/MessageHistoryGenerator.hpp"
    "templates/MessageReplyTemplates.hpp"
)

add_library(message-reply-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(message-reply-module
    LINK_PUBLIC common
)
target_include_directories(message-reply-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(message-reply-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_BUILD_TESTS})
    set(MESSAGE_REPLY_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-reply-module)
endif()
