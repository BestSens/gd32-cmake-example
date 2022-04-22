add_library(${EXEC_NAME}_gd32f450z_eval EXCLUDE_FROM_ALL
	gd32f450z_eval.c
)

target_include_directories(${EXEC_NAME}_gd32f450z_eval PUBLIC
	.
)

add_library(${EXEC_NAME}_gd32f450i_eval EXCLUDE_FROM_ALL
	gd32f450i_eval.c
)

target_include_directories(${EXEC_NAME}_gd32f450i_eval PUBLIC
	.
)

target_link_libraries(${EXEC_NAME}_gd32f450z_eval ${EXEC_NAME}_CMSIS)
target_link_libraries(${EXEC_NAME}_gd32f450i_eval ${EXEC_NAME}_CMSIS)