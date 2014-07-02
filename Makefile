TARGET_CHIP := NRF51822_QFAA_CA
BOARD := BOARD_NRF6310
CFLAGS += -DNRF51822_QFAA_CA -DBLE_STACK_SUPPORT_REQD -DBOARD_NRF6310 -Wno-unused-local-typedefs -Wno-unused-function
LDFLAGS += --specs=nano.specs

SDK_PATH := /opt/nordic/sdk/


C_SOURCE_FILES += main.c
C_SOURCE_FILES += $(SDK_PATH)Source/nrf_delay/nrf_delay.c
C_SOURCE_FILES += $(SDK_PATH)Source/spi_master/spi_master.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_advdata.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_bondmngr.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_conn_params.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_debug_assert_handler.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_error_log.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_radio_notification.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_sensorsim.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_services/ble_bas.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_services/ble_dis.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_services/ble_hrs.c
C_SOURCE_FILES += $(SDK_PATH)Source/ble/ble_services/ble_srv_common.c
C_SOURCE_FILES += $(SDK_PATH)Source/sd_common/softdevice_handler.c
C_SOURCE_FILES += $(SDK_PATH)Source/app_common/app_timer.c
C_SOURCE_FILES += $(SDK_PATH)Source/app_common/crc16.c
C_SOURCE_FILES += $(SDK_PATH)Source/app_common/pstorage.c

INCLUDEPATHS += -I"$(SDK_PATH)Include"
INCLUDEPATHS += -I"$(SDK_PATH)Include/ble"
INCLUDEPATHS += -I"$(SDK_PATH)Include/ble/ble_services"
INCLUDEPATHS += -I"$(SDK_PATH)Include/s110"
INCLUDEPATHS += -I"$(SDK_PATH)Include/app_common"
INCLUDEPATHS += -I"$(SDK_PATH)Include/sd_common"
OUTPUT_FILENAME := ble_hrs_gcc

DEVICE_VARIANT := xxaa
#DEVICE_VARIANT := xxab

USE_SOFTDEVICE := S110
#USE_SOFTDEVICE := s210

include $(SDK_PATH)Source/templates/gcc/Makefile.common
