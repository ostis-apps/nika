# добавление всех исходных файлов C++ в списки для цели сборки
set(SOURCES
	"MessageProcessingModule.cpp"
	"keynodes/MessageProcessingKeynodes.cpp"
	"agent/FindWordInSetByFirstLetterAgent.cpp"
)

set(HEADERS
	"MessageProcessingModule.hpp"
	"keynodes/MessageProcessingKeynodes.hpp"
	"agent/FindWordInSetByFirstLetterAgent.hpp"
)

# включение исходных файлов sc-памяти
include_directories(
	${CMAKE_CURRENT_LIST_DIR}
	${SC_MEMORY_SRC}
	${SC_KPM_SRC}
	${DIALOG_CONTROL_MODULE_SRC}
	${SC_COMMON_MODULE_SRC}
)

# создание библиотеки `messageProcessingModule` на базе собранных исходных файлов
add_library(messageProcessingModule SHARED ${SOURCES} ${HEADERS})

# линковка зависимости `messageProcessingModule` библиотек от 
# `sc-memory` и `sc-agents-common`
target_link_libraries(messageProcessingModule sc-memory sc-agents-common common dialogControlModule)

# запуск генератора кода (необходим для создания тел 
# sc-агентов и ключевых элементов)
sc_codegen_ex(messageProcessingModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
	target_clangformat_setup(messageProcessingModule)
endif()
