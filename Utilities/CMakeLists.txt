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

add_library(${EXEC_NAME}_lcd_gd32f450i_eval EXCLUDE_FROM_ALL
	LCD_common/lcd_font.c
	LCD_common/lcd_log.c
	gd32f450i_eval_exmc_sdram.c
	gd32f450i_lcd_eval.c
)

target_include_directories(${EXEC_NAME}_lcd_gd32f450i_eval PUBLIC
	.
	LCD_common
)

target_link_libraries(${EXEC_NAME}_lcd_gd32f450i_eval ${EXEC_NAME}_gd32f450i_eval ${EXEC_NAME}_usb_library_host)

add_subdirectory(Third_Party)