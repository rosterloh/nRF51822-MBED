# Vendor/device for which the library should be built.
MBED_DEVICE        := NRF51822
MBED_TARGET        := NRF51822_MKIT
MBED_CLEAN         := $(MBED_DEVICE)-MBED-clean

# Some libraries (mbed and rtos) have device specific source folders.
HAL_TARGET_SRC   := $(MBED_SRC_ROOT)/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822
HAL_TARGET_SRC   += $(MBED_SRC_ROOT)/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_NRF51822_MKIT
HAL_TARGET_SRC   += $(MBED_SRC_ROOT)/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/app_common
CMSIS_TARGET_SRC := $(MBED_SRC_ROOT)/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/
CMSIS_TARGET_SRC += $(MBED_SRC_ROOT)/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM
RTX_TARGET_SRC   := $(GCC4MBED_DIR)/mbed-src/libraries/rtos/rtx/TARGET_M0/TOOLCHAIN_GCC


# Compiler flags which are specifc to this device.
GCC_DEFINES := -DTARGET_NRF51822 -DTARGET_M0 -DTARGET_NORDIC -DTAGET_NRF51822_MKIT -DTARGET_MCU_NRF51822 -DTARGET_MCU_NORDIC_16K
GCC_DEFINES += -D__CORTEX_M0 -DARM_MATH_CM0

C_FLAGS   := -mcpu=cortex-m0 -mthumb -mthumb-interwork
ASM_FLAGS := -mcpu=cortex-m0 -mthumb
LD_FLAGS  := -mcpu=cortex-m0 -mthumb


# Linker script to be used.  Indicates what code should be placed where in memory.
LSCRIPT=$(GCC4MBED_DIR)/mbed-src/libraries/mbed/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_GCC_ARM/TARGET_MCU_NORDIC_16K/NRF51822.ld


include $(GCC4MBED_DIR)/build/device-common.mk
