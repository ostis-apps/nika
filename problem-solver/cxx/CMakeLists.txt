set(SC_COMMON_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}/commonModule")
set(DIALOG_CONTROL_MODULE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/dialogControlModule")
set(MESSAGE_REPLY_MODULE_SRC "${CMAKE_CURRENT_SOURCE_DIR}/messageReplyModule")

add_subdirectory(commonModule)
add_subdirectory(messageReplyModule)
add_subdirectory(dialogControlModule)
add_subdirectory(messageClassificationModule)
