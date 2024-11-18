make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message_processing_module_test_starter
    DEPENDS sc-agents-common messageProcessingModule sc-builder-lib sc-memory sc-core
    INCLUDES ${SC_MEMORY_SRC} ${SC_BUILDER_PATH} ${CMAKE_CURRENT_LIST_DIR}/..
)

add_definitions(-DMESSAGE_PROCESSING_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message_processing_module_test_starter)
endif()
