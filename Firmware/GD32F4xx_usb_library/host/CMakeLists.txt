add_library(${EXEC_NAME}_usb_library_host EXCLUDE_FROM_ALL
	../driver/Source/drv_usb_core.c
	../driver/Source/drv_usb_host.c
	../driver/Source/drv_usbh_int.c
	core/Source/usbh_core.c
	core/Source/usbh_enum.c
	core/Source/usbh_pipe.c
	core/Source/usbh_transc.c
)

target_include_directories(${EXEC_NAME}_usb_library_host PUBLIC
	core/Include
	../driver/Include
	../ustd/common
)

# hid
add_library(${EXEC_NAME}_usb_library_host_hid EXCLUDE_FROM_ALL
	class/hid/Source/usbh_hid_core.c
	class/hid/Source/usbh_standard_hid.c
)

target_include_directories(${EXEC_NAME}_usb_library_host_hid PUBLIC
	class/hid/Include
	../ustd/class/hid
)

target_link_libraries(${EXEC_NAME}_usb_library_host_hid ${EXEC_NAME}_usb_library_host)

# msc
add_library(${EXEC_NAME}_usb_library_host_msc EXCLUDE_FROM_ALL
	class/msc/Source/usbh_msc_bbb.c
	class/msc/Source/usbh_msc_core.c
	class/msc/Source/usbh_msc_fatfs.c
	class/msc/Source/usbh_msc_scsi.c
)

target_include_directories(${EXEC_NAME}_usb_library_host_msc PUBLIC
	class/msc/Include
	../ustd/class/msc
)

target_link_libraries(${EXEC_NAME}_usb_library_host_msc ${EXEC_NAME}_usb_library_host)