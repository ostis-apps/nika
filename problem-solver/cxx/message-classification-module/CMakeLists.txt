set(SOURCES
    "MessageClassificationModule.cpp"
    "agent/MessageTopicClassificationAgent.cpp"
    "agent/AlternativeMessageTopicClassificationAgent.cpp"
    "classifier/MessageTopicClassifier.cpp"
    "client/WitAiClient.cpp"
    "manager/MessageTopicClassificationManager.cpp"
    "constants/MessageClassificationConstants.cpp"
)

set(HEADERS
    "MessageClassificationModule.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "agent/MessageTopicClassificationAgent.hpp"
    "agent/AlternativeMessageTopicClassificationAgent.hpp"
    "classifier/MessageTopicClassifier.hpp"
    "client/WitAiClient.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "manager/MessageTopicClassificationManager.hpp"
    "client/WitAiClientInterface.hpp"
    "constants/MessageClassificationConstants.hpp"
)

find_package(nlohmann_json REQUIRED)

add_library(message-classification-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(message-classification-module
    LINK_PUBLIC sc-machine::sc-config
    LINK_PUBLIC scl-machine::inference
    LINK_PUBLIC common
    LINK_PUBLIC nlohmann_json::nlohmann_json
)
target_include_directories(message-classification-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(message-classification-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-classification-module)
endif()

if(${SC_BUILD_TESTS})
    set(MESSAGE_CLASSIFICATION_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()
