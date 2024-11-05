cmake_minimum_required(VERSION 3.11)
project(nika)
set(NIKA_ROOT ${CMAKE_CURRENT_LIST_DIR})

option(SC_BUILD_TESTS "Flag to build unit tests" OFF)
option(SC_CLANG_FORMAT_CODE "Flag to add clangformat and clangformat_check targets" OFF)

set(PROBLEM_SOLVER_PATH "${CMAKE_CURRENT_LIST_DIR}/problem-solver")
set(SC_MACHINE_PATH "${PROBLEM_SOLVER_PATH}/sc-machine")
add_subdirectory(${SC_MACHINE_PATH})
project_version(NIKA_PROJECT "0.1.0")

# platform doesn't support multi-extensions. Have to put project binaries to sc-machine extensions
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

# for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
endforeach(OUTPUT_CONFIG CMAKE_CONFIGURATION_TYPES)

macro(subdir_list result current_dir)
    file(GLOB children RELATIVE ${current_dir} ${current_dir}/*)
    set(dirlist "")
    foreach(child ${children})
        if(IS_DIRECTORY ${current_dir}/${child})
            list(APPEND dirlist ${child})
        endif()
    endforeach()
    set(${result} ${dirlist})
endmacro()

set(SUBSYSTEMS_PATH "${CMAKE_CURRENT_LIST_DIR}/subsystems")
subdir_list(SUBDIRS ${SUBSYSTEMS_PATH})

foreach(SUBDIR ${SUBDIRS})
    add_subdirectory(${SUBSYSTEMS_PATH}/${SUBDIR}/problem-solver)
endforeach()

add_subdirectory(problem-solver)
