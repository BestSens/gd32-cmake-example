cmake_minimum_required(VERSION 3.17.0)

include(example_set_name)

add_definitions(
	-DUSE_USB_HS
)

add_executable(${EXEC_NAME}
	src/gd32f4xx_it.c
	src/gd32f4xx_usb_hw.c
	src/main.c
	src/system_gd32f4xx.c
	src/usbh_usr_lcd.c
	src/usbh_usr.c
)

target_include_directories(${EXEC_NAME} PRIVATE
	inc
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Firmware Firmware)
add_subdirectory(${PROJECT_SOURCE_DIR}/Utilities Utilities)

target_include_directories(${EXEC_NAME}_CMSIS PRIVATE inc)
target_include_directories(${EXEC_NAME}_standard_peripherals PRIVATE inc)
target_include_directories(${EXEC_NAME}_usb_library_host PUBLIC inc)
target_include_directories(${EXEC_NAME}_gd32f450i_eval PUBLIC inc)

target_link_libraries(${EXEC_NAME}
	${EXEC_NAME}_CMSIS
	${EXEC_NAME}_standard_peripherals
	${EXEC_NAME}_usb_library_host_hid
	${EXEC_NAME}_lcd_gd32f450i_eval
)

target_link_libraries(${EXEC_NAME}_usb_library_host ${EXEC_NAME}_lcd_gd32f450i_eval)