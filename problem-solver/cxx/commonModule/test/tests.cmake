file(GLOB COMMON_TEST_AGENTS
    "${CMAKE_CURRENT_LIST_DIR}/agent/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/agent/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/keynodes/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/keynodes/*.hpp"
)
add_library(common_test_agents ${COMMON_TEST_AGENTS})
target_link_libraries(
    common_test_agents
    common
    sc-agents-common
)

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME common_test_starter
    DEPENDS common_test_agents sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${SC_COMMON_MODULE_SRC} ${SC_MACHINE_ROOT}/sc-tools
)

add_definitions(-DCOMMON_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(common_test_starter)
endif()
