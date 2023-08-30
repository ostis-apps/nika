make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message_processing_module_test_starter
    DEPENDS sc-agents-common messageProcessingModule sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_ROOT}/sc-tools
)

add_definitions(-DMESSAGE_PROCESSING_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
