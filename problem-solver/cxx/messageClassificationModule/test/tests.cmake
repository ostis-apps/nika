make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message_topic_classification_tests_starter
    DEPENDS sc-builder-lib sc-memory sc-core messageClassification sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC} ${SC_BUILDER_PATH} ${CMAKE_CURRENT_LIST_DIR}/..
)

add_definitions(-DMESSAGE_TOPIC_CLASSIFICATION_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message_topic_classification_tests_starter)
endif()
