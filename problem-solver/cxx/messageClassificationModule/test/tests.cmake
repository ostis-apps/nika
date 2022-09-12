make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message_topic_classification_tests_starter
    DEPENDS messageClassification sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_ROOT}/sc-tools/sc-builder/src
)

add_definitions(-DMESSAGE_TOPIC_CLASSIFICATION_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
