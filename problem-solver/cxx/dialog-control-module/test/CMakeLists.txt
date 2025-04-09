make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME dialog-control-module-tests
    DEPENDS dialog-control-module
    INCLUDES ${DIALOG_CONTROL_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(dialog-control-module-tests)
endif()
