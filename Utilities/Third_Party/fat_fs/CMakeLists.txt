add_library(${EXEC_NAME}_fat_fs EXCLUDE_FROM_ALL
	src/diskio.c
	src/fattime.c
	src/ff_gen_drv.c
	src/ff.c
	src/ffsystem.c
	src/ffunicode.c
)

target_include_directories(${EXEC_NAME}_fat_fs PUBLIC inc)
target_link_libraries(${EXEC_NAME}_fat_fs ${EXEC_NAME}_CMSIS)