file(GLOB non-atomic-action-interpreter-module_TEST_AGENTS
    "agent/*.cpp" "agent/*.hpp"
    "keynodes/*.cpp" "keynodes/*.hpp"
)

add_library(non-atomic-action-interpreter-module-test-agents SHARED ${non-atomic-action-interpreter-module_TEST_AGENTS})
target_link_libraries(non-atomic-action-interpreter-module-test-agents
    LINK_PUBLIC non-atomic-action-interpreter-module
)
target_include_directories(non-atomic-action-interpreter-module-test-agents
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(non-atomic-action-interpreter-module-test-agents)
endif()

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME non-atomic-action-interpreter-module-tests
    DEPENDS sc-machine::sc-builder-lib non-atomic-action-interpreter-module-test-agents
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${NON_ATOMIC_ACTION_INTERPRETER_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(non-atomic-action-interpreter-module-tests)
endif()
