make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message-processing-module-tests
    DEPENDS message-processing-module sc-machine::sc-builder-lib
    INCLUDES ${MESSAGE_PROCESSING_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-processing-module-tests)
endif()
