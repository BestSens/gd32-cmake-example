add_library(gd32f450z_eval
	gd32f450z_eval.c
)

target_include_directories(gd32f450z_eval PUBLIC
	.
)

add_library(gd32f450i_eval
	gd32f450i_eval.c
)

target_include_directories(gd32f450i_eval PUBLIC
	.
)

target_link_libraries(gd32f450z_eval CMSIS)
target_link_libraries(gd32f450i_eval CMSIS)