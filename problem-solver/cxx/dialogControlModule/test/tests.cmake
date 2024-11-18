make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME dialog_control_module_test_starter
    DEPENDS sc-builder-lib sc-memory sc-core sc-agents-common dialogControlModule
    INCLUDES ${SC_MEMORY_SRC} ${SC_BUILDER_PATH}  ${CMAKE_CURRENT_LIST_DIR}/..
)

add_definitions(-DDIALOG_CONTROL_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(dialog_control_module_test_starter)
endif()
