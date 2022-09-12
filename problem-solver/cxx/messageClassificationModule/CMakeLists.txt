set(SOURCES
    "MessageClassificationModule.cpp"
    "keynodes/MessageClassificationKeynodes.cpp"
    "agent/MessageTopicClassificationAgent.cpp"
    "classifier/MessageTopicClassifier.cpp"
    "client/WitAiClient.cpp"
    "keynodes/MessageClassificationKeynodes.cpp"
    "manager/MessageTopicClassificationManager.cpp"
    "constants/MessageClassificationConstants.cpp"
)

set(HEADERS
    "MessageClassificationModule.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "agent/MessageTopicClassificationAgent.hpp"
    "classifier/MessageTopicClassifier.hpp"
    "client/WitAiClient.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "manager/MessageTopicClassificationManager.hpp"
    "client/WitAiClientInterface.hpp"
    "constants/MessageClassificationConstants.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_COMMON_MODULE_SRC}
    ${DIALOG_CONTROL_MODULE_SRC}
    ${SC_TOOLS_SRC}/sc-config-utils
    ${GLIB2_INCLUDE_DIRS}
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
)

sc_codegen_ex(messageClassification ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${NIKA_ROOT}/nika.ini")

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(messageClassification)
endif()
