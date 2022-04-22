add_library(${EXEC_NAME}_usb_library_device EXCLUDE_FROM_ALL
	../driver/Source/drv_usb_core.c
	../driver/Source/drv_usb_dev.c
	../driver/Source/drv_usbd_int.c
	core/Source/usbd_core.c
	core/Source/usbd_enum.c
	core/Source/usbd_transc.c
)

target_include_directories(${EXEC_NAME}_usb_library_device PUBLIC
	core/Include
	../driver/Include
	../ustd/common
)

# audio
add_library(${EXEC_NAME}_usb_library_device_audio EXCLUDE_FROM_ALL
	class/audio/Source/audio_core.c
	class/audio/Source/audio_out_itf.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_audio PUBLIC
	class/audio/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_audio ${EXEC_NAME}_usb_library_device)

# cdc
add_library(${EXEC_NAME}_usb_library_device_cdc EXCLUDE_FROM_ALL
	class/cdc/Source/cdc_acm_core.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_cdc PUBLIC
	class/cdc/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_cdc ${EXEC_NAME}_usb_library_device)

# dfu
add_library(${EXEC_NAME}_usb_library_device_dfu EXCLUDE_FROM_ALL
	class/dfu/Source/dfu_core.c
	class/dfu/Source/dfu_mem.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_dfu PUBLIC
	class/dfu/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_dfu ${EXEC_NAME}_usb_library_device)

# hid
add_library(${EXEC_NAME}_usb_library_device_hid EXCLUDE_FROM_ALL
	class/hid/Source/custom_hid_core.c
	class/hid/Source/standard_hid_core.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_hid PUBLIC
	class/hid/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_hid ${EXEC_NAME}_usb_library_device)

# iap
add_library(${EXEC_NAME}_usb_library_device_iap EXCLUDE_FROM_ALL
	class/iap/Source/usb_iap_core.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_iap PUBLIC
	class/iap/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_iap ${EXEC_NAME}_usb_library_device)

# msc
add_library(${EXEC_NAME}_usb_library_device_msc EXCLUDE_FROM_ALL
	class/msc/Source/usbd_msc_bbb.c
	class/msc/Source/usbd_msc_core.c
	class/msc/Source/usbd_msc_scsi.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_msc PUBLIC
	class/msc/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_msc ${EXEC_NAME}_usb_library_device)

# printer
add_library(${EXEC_NAME}_usb_library_device_printer EXCLUDE_FROM_ALL
	class/printer/Source/printer_core.c
)

target_include_directories(${EXEC_NAME}_usb_library_device_printer PUBLIC
	class/printer/Include
)

target_link_libraries(${EXEC_NAME}_usb_library_device_printer ${EXEC_NAME}_usb_library_device)