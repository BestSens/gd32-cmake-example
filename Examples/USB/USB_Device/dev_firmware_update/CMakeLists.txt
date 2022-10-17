cmake_minimum_required(VERSION 3.17.0)

include(example_set_name)

add_definitions(
	-DUSE_USB_FS
)

add_executable(${EXEC_NAME}
	src/app.c
	src/flash_if.c
	src/exmc_nandflash.c
	src/gd25qxx.c
	src/inter_flash_if.c
	src/nand_flash_if.c
	src/nor_flash_if.c
	src/gd32f4xx_hw.c
	src/gd32f4xx_it.c
	src/system_gd32f4xx.c
)

target_include_directories(${EXEC_NAME} PRIVATE
	inc
)

add_subdirectory(${PROJECT_SOURCE_DIR}/Firmware Firmware)
add_subdirectory(${PROJECT_SOURCE_DIR}/Utilities Utilities)

target_include_directories(${EXEC_NAME}_CMSIS PRIVATE inc)
target_include_directories(${EXEC_NAME}_standard_peripherals PRIVATE inc)
target_include_directories(${EXEC_NAME}_usb_library_device PUBLIC inc)
target_include_directories(${EXEC_NAME}_gd32f450z_eval PRIVATE inc)

target_link_libraries(${EXEC_NAME}
	${EXEC_NAME}_CMSIS
	${EXEC_NAME}_standard_peripherals
	${EXEC_NAME}_usb_library_device_dfu
	${EXEC_NAME}_gd32f450z_eval
)

target_link_libraries(${EXEC_NAME}_usb_library_device ${EXEC_NAME}_gd32f450z_eval)