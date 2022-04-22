cmake_minimum_required(VERSION 3.17.0)

set(EXEC_NAME Keyboard_polling_mode)

add_executable(${EXEC_NAME}
	gd32f4xx_it.c
	main.c
	systick.c
)

target_include_directories(${EXEC_NAME} PRIVATE
	.
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Firmware ${EXEC_NAME}/Firmware)
add_subdirectory(${PROJECT_SOURCE_DIR}/Utilities ${EXEC_NAME}/Utilities)

target_include_directories(${EXEC_NAME}_CMSIS PRIVATE .)
target_include_directories(${EXEC_NAME}_standard_peripherals PRIVATE .)
target_include_directories(${EXEC_NAME}_gd32f450z_eval PRIVATE .)

target_link_libraries(${EXEC_NAME} ${EXEC_NAME}_CMSIS ${EXEC_NAME}_standard_peripherals ${EXEC_NAME}_gd32f450z_eval)