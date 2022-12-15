set(SC_SET_AGENTS_SRC ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")

include_directories(
  ${SC_SET_AGENTS_SRC} 
  ${SC_MEMORY_SRC} 
  ${SC_KPM_SRC}
  ${SC_TOOLS_SRC}
  ${CMAKE_CURRENT_LIST_DIR}/../commonModule
)

add_library(mathModule SHARED ${SOURCES} ${HEADERS})

add_dependencies(
  mathModule
  sc-memory
  sc-agents-common
  common
)

target_link_libraries(
  mathModule
  sc-memory
  sc-agents-common
  common
)

sc_codegen_ex(mathModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

add_definitions(-DMATH_CONSTANTS_RESOURCES="${CMAKE_CURRENT_LIST_DIR}/resources/math_constants.json")
