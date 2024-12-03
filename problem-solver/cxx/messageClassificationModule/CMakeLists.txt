set(INFERENCE_MODULE_SRC "${SUBSYSTEMS_PATH}/scl-machine/problem-solver/cxx/inferenceModule")

set(SOURCES
    "MessageClassificationModule.cpp"
    "agent/WitMessageTopicClassificationAgent.cpp"
    "agent/RasaMessageTopicClassificationAgent.cpp"
    "agent/AlternativeMessageTopicClassificationAgent.cpp"
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
    # "agent/MessageTopicClassificationAgentInterface.hpp"
    "agent/WitMessageTopicClassificationAgent.hpp"
    "agent/RasaMessageTopicClassificationAgent.hpp"
    "agent/AlternativeMessageTopicClassificationAgent.hpp"
    "classifier/WitMessageTopicClassifier.hpp"
    "classifier/RasaMessageTopicClassifier.hpp"
    "client/ClientInterface.hpp"
    "client/WitAiClient.hpp"
    "client/RasaClient.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "manager/MessageTopicClassificationManagerInterface.hpp"
    "manager/WitMessageTopicClassificationManager.hpp"
    "manager/RasaMessageTopicClassificationManager.hpp"
    "constants/WitMessageClassificationConstants.hpp"
    "constants/RasaMessageClassificationConstants.hpp"
    "helper/MessageClassificationHelper.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_COMMON_MODULE_SRC}
    ${DIALOG_CONTROL_MODULE_SRC}
    ${SC_TOOLS_SRC}/sc-config-utils
    ${GLIB2_INCLUDE_DIRS}
    ${INFERENCE_MODULE_SRC}
)

add_library(messageClassification SHARED ${SOURCES} ${HEADERS})
target_link_libraries(
    messageClassification
    sc-memory
    common
    sc-agents-common
    dialogControlModule
    messageReply
    sc-config-utils
	inferenceModule
)

add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${NIKA_ROOT}/nika.ini")

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(messageClassification)
endif()
