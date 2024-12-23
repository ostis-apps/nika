set(INFERENCE_MODULE_SRC "${SUBSYSTEMS_PATH}/scl-machine/problem-solver/cxx/inferenceModule")

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

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_COMMON_MODULE_SRC}
    ${DIALOG_CONTROL_MODULE_SRC}
    ${GLIB2_INCLUDE_DIRS}
    ${INFERENCE_MODULE_SRC}
    ${SC_AGENTS_COMMON_PATH}
    ${SC_CORE_PATH}
    ${SC_BUILDER_PATH}
)

add_library(messageClassification SHARED ${SOURCES} ${HEADERS})
target_link_libraries(
    messageClassification
    sc-memory
    common
    sc-agents-common
    dialogControlModule
    messageReply
    sc-config
	inferenceModule
)

set_target_properties(messageClassification PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS})

add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${NIKA_ROOT}/nika.ini")

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(messageClassification)
endif()
