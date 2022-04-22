cmake_minimum_required(VERSION 3.17.0)

include(example_set_name)

add_executable(${EXEC_NAME}
	exmc_sdram.c
	gd32f4xx_it.c
	main.c
	systick.c
)

target_include_directories(${EXEC_NAME} PRIVATE
	.
)

include(example_add_libs)