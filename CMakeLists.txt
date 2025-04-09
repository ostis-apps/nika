cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)
project(nika VERSION 0.2.0 LANGUAGES C CXX)
message(STATUS "Current project version: ${CMAKE_PROJECT_VERSION}")
cmake_policy(SET CMP0048 NEW)

if(${WIN32})
    message(FATAL_ERROR "NIKA isn't supported on Windows OS.")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(DEFINED SC_EXTENSIONS_DIRECTORY)
    message(STATUS "Provided extensions directory: ${SC_EXTENSIONS_DIRECTORY}")
    file(MAKE_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
else()
    set(SC_EXTENSIONS_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/extensions)
    file(MAKE_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
    message(STATUS "No extensions directory provided. Defaulting to: ${SC_EXTENSIONS_DIRECTORY}")
endif()

option(SC_CLANG_FORMAT_CODE "Flag to add clangformat and clangformat_check targets" OFF)
option(SC_BUILD_TESTS "Flag to build unit tests" OFF)

option(AUTO_CCACHE "Use ccache to speed up rebuilds" ON)

if(${SC_BUILD_TESTS})
    include(CTest)
    include(GoogleTest)
    find_package(GTest REQUIRED)
    include(${CMAKE_MODULE_PATH}/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM AND ${AUTO_CCACHE})
    message(STATUS "Using ${CCACHE_PROGRAM} as compiler launcher")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG)

include(GNUInstallDirs)

message("CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message("Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}, path: ${CMAKE_CXX_COMPILER}")

# Set up dynamic linking paths for our own libraries
# For macOS, use @loader_path instead of $ORIGIN
if(APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path;@loader_path/../lib;")
else()
    set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib;")
endif()

# Always use full RPATH
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

find_package(sc-machine REQUIRED)
find_package(scl-machine REQUIRED)

add_subdirectory(problem-solver)

include(${CMAKE_MODULE_PATH}/install.cmake)
