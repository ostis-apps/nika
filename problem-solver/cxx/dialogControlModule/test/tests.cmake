make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME test_starter
    DEPENDS sc-agents-common dialogControlModule sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_ROOT}/sc-tools
)

add_definitions(-DDIALOG_CONTROL_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(test_starter)
endif()
