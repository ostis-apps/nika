set(SOURCES
    "DialogControlModule.cpp"
    "generator/MessageConstructionsGenerator.cpp"
    "agent/PhraseGenerationAgent.cpp"
    "agent/StandardMessageReplyAgent.cpp"
    "searcher/LanguageSearcher.cpp"
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
    "searcher/PhraseSearcher.hpp"
    "searcher/TokenDomainSearcher.hpp"
    "handler/MessageHandler.hpp"
)

add_library(dialog-control-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(dialog-control-module
    LINK_PUBLIC sc-machine::sc-builder-lib
    LINK_PUBLIC scl-machine::inference
    LINK_PUBLIC common
)
target_include_directories(dialog-control-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(dialog-control-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_BUILD_TESTS})
    set(DIALOG_CONTROL_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(dialog-control-module)
endif()
