add_library(CMSIS EXCLUDE_FROM_ALL
	GD/GD32F4xx/Source/GNU/startup_gd32f450_470.s
	GD/GD32F4xx/Source/system_gd32f4xx.c
)

target_include_directories(CMSIS PUBLIC
	.
	GD/GD32F4xx/Include
)

target_link_libraries(CMSIS standard_peripherals)