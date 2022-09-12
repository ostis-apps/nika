file(GLOB REPLY_MODULE_TEST_AGENTS
    "${CMAKE_CURRENT_LIST_DIR}/agent/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/agent/*.hpp"
)
add_library(replyModuleTestAgents ${REPLY_MODULE_TEST_AGENTS})
target_link_libraries(
    replyModuleTestAgents
    messageReply
    sc-agents-common
    common
)

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message_reply_module_test_starter
    DEPENDS sc-agents-common messageReply sc-builder-lib replyModuleTestAgents common
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_ROOT}/sc-tools
)

add_definitions(-DMESSAGE_REPLY_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
