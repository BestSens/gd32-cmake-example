add_library(${EXEC_NAME}_CMSIS EXCLUDE_FROM_ALL
	GD/GD32F4xx/Source/GNU/startup_gd32f450.cpp
	GD/GD32F4xx/Source/system_gd32f4xx.c
)

target_include_directories(${EXEC_NAME}_CMSIS PUBLIC
	.
	GD/GD32F4xx/Include
)

target_link_libraries(${EXEC_NAME}_CMSIS ${EXEC_NAME}_standard_peripherals)