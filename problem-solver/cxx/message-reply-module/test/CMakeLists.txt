file(GLOB REPLY_MODULE_TEST_AGENTS
    "agent/*.cpp" "agent/*.hpp"
)

add_library(message-reply-module-test-agents SHARED ${REPLY_MODULE_TEST_AGENTS})
target_link_libraries(message-reply-module-test-agents
    LINK_PUBLIC sc-machine::sc-builder-lib
    LINK_PUBLIC common
    LINK_PUBLIC message-reply-module
)
target_include_directories(message-reply-module-test-agents
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${MESSAGE_REPLY_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-reply-module-test-agents)
endif()

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message-reply-module-tests
    DEPENDS message-reply-module-test-agents
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${MESSAGE_REPLY_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-reply-module-tests)
endif()
