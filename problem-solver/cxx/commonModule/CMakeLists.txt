set(SOURCES
    "CommonModule.cpp"
    "keynodes/Keynodes.cpp"
    "agent/NonAtomicActionInterpreterAgent.cpp"
    "interpreter/NonAtomicActionInterpreter.cpp"
    "handler/NumberHandler.cpp"
    "handler/LinkHandler.cpp"
    "handler/ParameterHandler.cpp"
    "utils/RelationUtils.cpp"
    "utils/ActionUtils.cpp"
    "utils/ScTemplateUtils.cpp"
    "http/sc_http.cpp"
    "http/sc_http_request.cpp"
    "http/sc_http_response.cpp"
)

set(HEADERS
    "CommonModule.hpp"
    "keynodes/Keynodes.hpp"
    "agent/NonAtomicActionInterpreterAgent.hpp"
    "interpreter/NonAtomicActionInterpreter.hpp"
    "handler/NumberHandler.hpp"
    "handler/LinkHandler.hpp"
    "handler/ParameterHandler.hpp"
    "utils/RelationUtils.hpp"
    "utils/ActionUtils.hpp"
    "utils/ScTemplateUtils.hpp"
    "http/sc_http.hpp"
    "http/sc_http_request.hpp"
    "http/sc_http_response.hpp"
)

include_directories(${CMAKE_CURRENT_LIST_DIR} ${SC_MEMORY_SRC} ${SC_KPM_SRC} ${SC_TOOLS_SRC})

add_library(common SHARED ${SOURCES} ${HEADERS})

find_package(nlohmann_json 3.7.3 REQUIRED)
target_link_libraries(common sc-memory sc-agents-common nlohmann_json::nlohmann_json)

sc_codegen_ex(common ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(common)
endif()
