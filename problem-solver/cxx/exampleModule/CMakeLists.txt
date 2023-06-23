# добавление всех исходников, оканчивающихся на ".cpp" и 
# ".hpp", в списки для цели сборки
set(SOURCES
	"ExampleModule.cpp"
	"keynodes/LetterKeynodes.cpp"
	"agent/LetterAgent.cpp"
)

set(HEADERS
	"ExampleModule.hpp"
	"keynodes/LetterKeynodes.hpp"
	"agent/LetterAgent.hpp"
)

# включение исходников sc-памяти
include_directories(
	${CMAKE_CURRENT_LIST_DIR}
	${SC_MEMORY_SRC}
	${SC_KPM_SRC}
	${DIALOG_CONTROL_MODULE_SRC}
	${SC_COMMON_MODULE_SRC}
)

# создание библиотеки `exampleModule` на базе собранных 
# исходников
add_library(exampleModule SHARED ${SOURCES} ${HEADERS})

# линковка зависимости `exampleModule` библиотек от 
# `sc-memory` и `sc-agents-common`
target_link_libraries(exampleModule sc-memory sc-agents-common common dialogControlModule)

# запуск генератора кода (необходим для создания тел 
# sc-агентов и ключевых элементов)
sc_codegen_ex(exampleModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
	target_clangformat_setup(exampleModule)
endif()
