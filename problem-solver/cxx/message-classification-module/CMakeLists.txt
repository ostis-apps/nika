set(SOURCES
    "MessageClassificationModule.cpp"
    "agent/AlternativeMessageTopicClassificationAgent.cpp"
    "agent/WitMessageTopicClassificationAgent.cpp"
    "agent/RasaMessageTopicClassificationAgent.cpp"
    "classifier/WitMessageTopicClassifier.cpp"
    "classifier/RasaMessageTopicClassifier.cpp"
    "client/WitAiClient.cpp"
    "client/RasaClient.cpp"
    "manager/WitMessageTopicClassificationManager.cpp"
    "manager/RasaMessageTopicClassificationManager.cpp"
    "constants/WitMessageClassificationConstants.cpp"
    "constants/RasaMessageClassificationConstants.cpp"
    "helper/MessageClassificationHelper.cpp"
)

set(HEADERS
    "MessageClassificationModule.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "agent/AlternativeMessageTopicClassificationAgent.hpp"
    "agent/WitMessageTopicClassificationAgent.hpp"
    "agent/RasaMessageTopicClassificationAgent.hpp"
    "classifier/WitMessageTopicClassifier.hpp"
    "classifier/RasaMessageTopicClassifier.hpp"
    "client/WitAiClient.hpp"
    "client/RasaClient.hpp"
    "client/ClientInterface.hpp"
    "manager/WitMessageTopicClassificationManager.hpp"
    "manager/RasaMessageTopicClassificationManager.hpp"
    "manager/MessageTopicClassificationManagerInterface.hpp"
    "constants/WitMessageClassificationConstants.hpp"
    "constants/RasaMessageClassificationConstants.hpp"
    "helper/MessageClassificationHelper.hpp"
)

# Use Conan-provided packages
find_package(nlohmann_json REQUIRED)
find_package(sc-machine REQUIRED)
find_package(scl-machine REQUIRED)

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

# Optional definition from your old version
add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${NIKA_ROOT}/nika.ini")

set_target_properties(message-classification-module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-classification-module)
endif()

if(${SC_BUILD_TESTS})
    set(MESSAGE_CLASSIFICATION_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()

