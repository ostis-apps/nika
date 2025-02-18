set(SOURCES
    "NonAtomicActionInterpreterModule.cpp"
    "agent/NonAtomicActionInterpreterAgent.cpp"
    "interpreter/NonAtomicActionInterpreter.cpp"
)

set(HEADERS
    "NonAtomicActionInterpreterModule.hpp"
    "agent/NonAtomicActionInterpreterAgent.hpp"
    "interpreter/NonAtomicActionInterpreter.hpp"
)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

add_library(non-atomic-action-interpreter-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(non-atomic-action-interpreter-module
    LINK_PUBLIC common
)
target_include_directories(non-atomic-action-interpreter-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(non-atomic-action-interpreter-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(non-atomic-action-interpreter-module)
endif()

if(${SC_BUILD_TESTS})
    set(NON_ATOMIC_ACTION_INTERPRETER_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()
