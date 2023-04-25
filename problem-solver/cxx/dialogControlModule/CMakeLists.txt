set(INFERENCE_MODULE_SRC "${SUBSYSTEMS_PATH}/ostis-inference/problem-solver/cxx/inferenceModule")

set(SOURCES
    "DialogControlModule.cpp"
    "keynodes/DialogKeynodes.cpp"
    "keynodes/MessageKeynodes.cpp"
    "generator/MessageConstructionsGenerator.cpp"
    "agent/PhraseGenerationAgent.cpp"
    "agent/StandardMessageReplyAgent.cpp"
    "searcher/LanguageSearcher.cpp"
    "searcher/MessageSearcher.cpp"
    "searcher/PhraseSearcher.cpp"
    "searcher/TokenDomainSearcher.cpp"
    "handler/MessageHandler.cpp"
)

set(HEADERS
    "DialogControlModule.hpp"
    "keynodes/DialogKeynodes.hpp"
    "keynodes/MessageKeynodes.hpp"
    "generator/MessageConstructionsGenerator.hpp"
    "agent/PhraseGenerationAgent.hpp"
    "agent/StandardMessageReplyAgent.hpp"
    "searcher/LanguageSearcher.hpp"
    "searcher/MessageSearcher.hpp"
    "searcher/PhraseSearcher.hpp"
    "searcher/TokenDomainSearcher.hpp"
    "handler/MessageHandler.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}
    ${INFERENCE_MODULE_SRC}
    ${SC_COMMON_MODULE_SRC}
)

add_library(dialogControlModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(dialogControlModule sc-memory sc-agents-common common sc-builder-lib inferenceModule)

sc_codegen_ex(dialogControlModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(dialogControlModule)
endif()
