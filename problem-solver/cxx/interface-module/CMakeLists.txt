set(SOURCES
    "InterfaceModule.cpp"
    "agent/ChangeInterfaceColorAgent.cpp"
)

set(HEADERS
    "InterfaceModule.hpp"
    "keynodes/InterfaceKeynodes.hpp"
    "agent/ChangeInterfaceColorAgent.hpp"
)

add_library(interface-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(interface-module
    LINK_PUBLIC common
)
target_include_directories(interface-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(interface-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(interface-module)
endif()
