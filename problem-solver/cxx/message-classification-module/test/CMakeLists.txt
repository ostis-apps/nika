make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message-topic-classification-module-tests
    DEPENDS sc-machine::sc-builder-lib message-classification-module GTest::gmock_main
    INCLUDES ${MESSAGE_CLASSIFICATION_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-topic-classification-module-tests)
endif()
