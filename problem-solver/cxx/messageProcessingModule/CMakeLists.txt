set(SOURCES
	"MessageProcessingModule.cpp"
	"agent/FindWordInSetByFirstLetterAgent.cpp"
)

set(HEADERS
	"MessageProcessingModule.hpp"
	"keynodes/MessageProcessingKeynodes.hpp"
	"agent/FindWordInSetByFirstLetterAgent.hpp"
)

include_directories(
	${CMAKE_CURRENT_LIST_DIR}
	${SC_MEMORY_SRC}
	${SC_AGENTS_COMMON_PATH}
	${SC_CORE_PATH}
	${SC_BUILDER_PATH}
	${DIALOG_CONTROL_MODULE_SRC}
	${SC_COMMON_MODULE_SRC}
)

add_library(messageProcessingModule SHARED ${SOURCES} ${HEADERS})

target_link_libraries(messageProcessingModule sc-memory sc-agents-common common dialogControlModule)

set_target_properties(messageProcessingModule PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS})

if(${SC_CLANG_FORMAT_CODE})
	target_clangformat_setup(messageProcessingModule)
endif()

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()
