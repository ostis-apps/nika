set(SOURCES
	"MessageProcessingModule.cpp"
	"agent/FindWordInSetByFirstLetterAgent.cpp"
)

set(HEADERS
	"MessageProcessingModule.hpp"
	"keynodes/MessageProcessingKeynodes.hpp"
	"agent/FindWordInSetByFirstLetterAgent.hpp"
)

add_library(message-processing-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(message-processing-module
	LINK_PUBLIC common
)
target_include_directories(message-processing-module
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(message-processing-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
	target_clangformat_setup(message-processing-module)
endif()

if(${SC_BUILD_TESTS})
	set(MESSAGE_PROCESSING_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
	add_subdirectory(test)
endif()
