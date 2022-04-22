cmake_minimum_required(VERSION 3.17.0)

set(EXEC_NAME Running_led)

add_executable(${EXEC_NAME}
	gd32f4xx_it.c
	main.c
	systick.c
)

target_include_directories(${EXEC_NAME} PRIVATE
	.
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Firmware/CMSIS ${EXEC_NAME}/CMSIS)
add_subdirectory(${PROJECT_SOURCE_DIR}/Firmware/GD32F4xx_standard_peripheral ${EXEC_NAME}/GD32F4xx)
add_subdirectory(${PROJECT_SOURCE_DIR}/Utilities ${EXEC_NAME}/Utilities)

target_include_directories(CMSIS PRIVATE .)
target_include_directories(standard_peripherals PRIVATE .)
target_include_directories(gd32f450z_eval PRIVATE .)

target_link_libraries(${EXEC_NAME} CMSIS standard_peripherals gd32f450z_eval)