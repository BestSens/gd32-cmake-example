cmake_minimum_required(VERSION 3.17.0)

include(example_set_name)

set(LWIP_INCLUDE_DIRS
	${CMAKE_CURRENT_LIST_DIR}/lwip-2.2.0/src/include
	${CMAKE_CURRENT_LIST_DIR}/lwip-2.2.0/port/GD32F4xx
	${CMAKE_CURRENT_LIST_DIR}/inc
)

add_subdirectory(lwip-2.2.0)

add_executable(${EXEC_NAME}
	src/gd32f4xx_enet_eval.c
	src/gd32f4xx_it.c
	src/hello_gigadevice.c
	src/main.c
	src/netconf.c
	${CMAKE_SOURCE_DIR}/Retarget/retarget.c
)

target_include_directories(${EXEC_NAME} PRIVATE
	inc
)

add_library(lwip_port
	lwip-2.2.0/port/GD32F4xx/Basic/ethernetif.c
)

target_include_directories(lwip_port PUBLIC
	${LWIP_INCLUDE_DIRS}
	lwip-2.2.0/src/include/lwip
	lwip-2.2.0/port/GD32F4xx/Basic
)
target_link_libraries(lwip_port lwipcore ${EXEC_NAME}_standard_peripherals)

add_subdirectory(${PROJECT_SOURCE_DIR}/Firmware Firmware)
add_subdirectory(${PROJECT_SOURCE_DIR}/Utilities Utilities)

target_include_directories(${EXEC_NAME}_CMSIS PRIVATE inc)
target_include_directories(${EXEC_NAME}_standard_peripherals PRIVATE inc)
target_include_directories(${EXEC_NAME}_gd32f450z_eval PRIVATE inc)


target_link_libraries(${EXEC_NAME}
	${EXEC_NAME}_CMSIS
	${EXEC_NAME}_standard_peripherals
	${EXEC_NAME}_gd32f450z_eval
	lwip_port
)