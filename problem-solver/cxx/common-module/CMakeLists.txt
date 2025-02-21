file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*/*.hpp" "src/*/*.cpp"
    "include/common/*/*.hpp" "include/common/*/*.cpp"
)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)

add_library(common SHARED ${SOURCES})
target_link_libraries(common
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PRIVATE nlohmann_json::nlohmann_json
    LINK_PRIVATE CURL::libcurl
)
target_include_directories(common
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(common)
endif()
