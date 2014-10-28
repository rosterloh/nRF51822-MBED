#
# CMake configuration
#
# Please refer to http://www.cmake.org/cmake/help/v3.0/
# You may also refer to http://www.cmake.org/cmake/help/v3.0/manual/cmake-language.7.html for a quick
# introduction to CMake's syntax.
# set the arm toolchain
set(CMAKE_TOOLCHAIN_FILE ../arm_toolchain.cmake)

cmake_minimum_required (VERSION 3.0.1)

# The name of our project is "BLE_NUDGE". CMakeLists files in this project can
# refer to the root source directory of the project as ${BLE_NUDGE_SOURCE_DIR}
# and to the root binary directory of the project as ${BLE_NUDGE_BINARY_DIR}.
project (BLE_NUDGE C CXX ASM)

# The version number.
set (BLE_NUDGE_VERSION_MAJOR 1)
set (BLE_NUDGE_VERSION_MINOR 0)
set (BLE_NUDGE_VERSION_PATCH 0)
set (BLE_NUDGE_VERSION_TWEAK 0)

# define some more paths to projects we depend on
set (MBED_SRC_PATH     ${BLE_NUDGE_SOURCE_DIR}/../../mbed-src/libraries/mbed)
set (BLE_API_SRC_PATH  ${BLE_NUDGE_SOURCE_DIR}/../../BLE_API)
set (NRF51822_SRC_PATH ${BLE_NUDGE_SOURCE_DIR}/../../nRF51822)

set(MBED_VENDOR "NORDIC")
set(MBED_FAMILY "MCU_NRF51822")
set(MBED_CPU "NRF51822_MKIT")
set(MBED_CORE "cortex-m0")
set(MBED_INSTRUCTIONSET "M0")

set(MBED_STARTUP "startup_NRF51822.o")
set(MBED_SYSTEM "system_nrf51822.o")
set(MBED_LINK_TARGET "NRF51822")

# It's best to hide all the details of setting up the variable SRCS in a CMake
# macro. The macro can then be called in all the project CMake list files to add
# sources.
#
# The macro first computes the path of the source file relative to the project
# root for each argument. If the macro is invoked from inside a project sub
# directory the new value of the variable SRCS needs to be propagated to the
# parent folder by using the PARENT_SCOPE option.
#
# Source: http://stackoverflow.com/questions/7046956/populating-srcs-from-cmakelists-txt-in-subdirectories
macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()

# decide about the actual compilers to be used ...
set(TOOLCHAIN TOOLCHAIN_GCC_ARM)
if( CMAKE_HOST_WIN32 )
  set(TOOLCHAIN_SYSROOT  "C:\\ProgramData\\chocolatey\\lib\\gcc-arm-embedded.4.8.2014.3\\tools")
  set(CMAKE_CXX_COMPILER C:\ProgramData\chocolatey\bin\arm-none-eabi-g++)
  set(CMAKE_C_COMPILER   C:\ProgramData\chocolatey\bin\arm-none-eabi-gcc)
  set(SIZE_COMMAND       C:\ProgramData\chocolatey\bin\arm-none-eabi-size)
  set(OBJCOPY_COMMAND    C:\ProgramData\chocolatey\bin\arm-none-eabi-objcopy)
else()
  set(TOOLCHAIN_SYSROOT  "/opt/gcc-arm")
  set(CMAKE_CXX_COMPILER ${TOOLCHAIN_SYSROOT}/bin/arm-none-eabi-g++)
  set(CMAKE_C_COMPILER   ${TOOLCHAIN_SYSROOT}/bin/arm-none-eabi-gcc)
  set(SIZE_COMMAND       ${TOOLCHAIN_SYSROOT}/bin/arm-none-eabi-size)
  set(OBJCOPY_COMMAND    ${TOOLCHAIN_SYSROOT}/bin/arm-none-eabi-objcopy)
endif()
set(MAIN_TARGET        ${PROJECT_NAME}.elf)
#enable_language(ASM)

message(STATUS "C compiler  : ${CMAKE_C_COMPILER}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Size command: ${SIZE_COMMAND}")
message(STATUS "Main target : ${MAIN_TARGET}")

############################################################################
# Build type should be clear from here so we
# can continue with selecting include directors, defines
# and other compiler/linker flags ...
############################################################################
# include directories
include_directories(
    ${BLE_NUDGE_SOURCE_DIR}
    ${MBED_SRC_PATH}/
    ${MBED_SRC_PATH}/api
    ${MBED_SRC_PATH}/common
    ${MBED_SRC_PATH}/hal
    ${MBED_SRC_PATH}/targets
    ${MBED_SRC_PATH}/targets/cmsis
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/${TOOLCHAIN}
    ${MBED_SRC_PATH}/targets/hal
    ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}
    ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}
    ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/Lib
    ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TARGET_${MBED_CPU}
    ${BLE_API_SRC_PATH}
    ${BLE_API_SRC_PATH}/public
    ${BLE_API_SRC_PATH}/common
    ${BLE_API_SRC_PATH}/services
    ${NRF51822_SRC_PATH}
    ${NRF51822_SRC_PATH}/btle
    ${NRF51822_SRC_PATH}/btle/custom
    ${NRF51822_SRC_PATH}/common
    ${NRF51822_SRC_PATH}/nordic
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/app_common
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/ble
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/ble/ble_services
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/ble/rpc
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/s110
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/sd_common
    ${NRF51822_SRC_PATH}/nordic/nrf-sdk/bootloader_dfu
)

# ------------------------------------------------------------------------------
# compiler settings
#SET(COMMON_FLAGS "${COMMON_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-exceptions -fno-builtin -MMD -fno-delete-null-pointer-checks")
#SET(COMMON_FLAGS "${COMMON_FLAGS} -mcpu=${MBED_CORE} -O3 -mthumb -fno-exceptions -msoft-float -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")

#SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_${MBED_TARGET}")
#SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_${MBED_INSTRUCTIONSET}")
#SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_${MBED_VENDOR}")
#SET(MBED_DEFINES "${MBED_DEFINES} -DTOOLCHAIN_GCC_ARM")
#SET(MBED_DEFINES "${MBED_DEFINES} -DTOOLCHAIN_GCC")

#SET(MBED_DEFINES "${MBED_DEFINES} -D__CORTEX_M0")
#SET(MBED_DEFINES "${MBED_DEFINES} -DARM_MATH_CM0")
#SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_ARCH_BLE")
#SET(MBED_DEFINES "${MBED_DEFINES} -DTARGET_MCU_NRF51822")
#SET(MBED_DEFINES "${MBED_DEFINES} -D__MBED__=1")
#SET(MBED_DEFINES "${MBED_DEFINES} -DMBED_USERNAME=rosterloh84")
#SET(MBED_DEFINES "${MBED_DEFINES} -DMBED_BUILD_TIMESTAMP=1414140006.00")

#SET(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${MBED_DEFINES} -std=gnu++0x")
#SET(CMAKE_C_FLAGS "${COMMON_FLAGS} ${MBED_DEFINES} -std=gnu99")

# Generic compiler flags
add_definitions(
    -mcpu=cortex-m0
    -mthumb
    -Wabi
    -Wno-error=switch
    -Wno-switch
    -fdata-sections
    -ffunction-sections
    -fmessage-length=0
    -Wall
    -fno-common
    -fno-exceptions
    -funsigned-bitfields
    -O3
    -DTARGET_NRF51822
    -DTARGET_M0
    -DTARGET_NORDIC
    -DTOOLCHAIN_GCC
    -D__CORTEX_M0
    -DARM_MATH_CM0
    -D__MBED__=1
    -DTARGET_ARCH_BLE
    -DTARGET_MCU_NRF51822
    -DTOOLCHAIN_GCC_ARM
    -DMBED_BUILD_TIMESTAMP=1414140006.00
    -DMBED_USERNAME=rosterloh84
    -Wa,-adhlns=$@.lst
    -g3
    -ggdb
    -Wl,--gc-sections
    -Wl,--wrap=main
    --specs=nano.specs
    -lstdc++
    -lsupc++
    -lm
    -lc
    -lgcc
    -lnosys
)
# -mabi=aapcs
# -mfloat-abi=soft
# -fno-builtin

# Language specifc compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-threadsafe-statics")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu99 -Wno-pointer-sign -Wno-pointer-to-int-cast")
set(CMAKE_ASM_FLAGS "${COMMON_COMPILE_FLAGS} -x assembler-with-cpp")

SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
-T${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/${TOOLCHAIN}/NRF51822.ld)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS
    "${CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS} -mcpu=cortex-m0 -mthumb -Wl,--gc-sections")

# A macro to collect local sources into ${SRCS}.
# This variable gets propagated to the parent scope and is ultimately used in
# the top-level CMakeLists.txt to define the dependencies for the build target.
#
# Please note that files within this list are relative to the current folder.
# Please also note that this macro must be used at all CMakeLists.txt files at
# intermediate levels even if the list is empty--this is due to the Cmake magic
# involved in propagating variables to only the parent scope.
add_sources(
    main.cpp
)

# Use file globbing to collect all sources from external repositories. File-
# globbing is discouraged by CMake, except when collecting sources from an
# external source which remains mostly frozen. The risk with globbing is that
# CMake doesn't automatically update the makefiles if new sources are added to
# the globbed location.
#
file(GLOB MBED_SRC_SOURCES
          ${MBED_SRC_PATH}/common/*.c
          ${MBED_SRC_PATH}/common/*.cpp
          ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/*.c
          ${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/TOOLCHAIN_ARM_STD/sys.cpp
          ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/*.c
          ${MBED_SRC_PATH}/targets/hal/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/Lib/app_common/*.c
    )
add_sources(${MBED_SRC_SOURCES})
file(GLOB BLE_API_SOURCES
          ${BLE_API_SRC_PATH}/common/*.cpp
          ${BLE_API_SRC_PATH}/services/*.cpp
    )
add_sources(${BLE_API_SOURCES})
file(GLOB NRF51822_SOURCES
          ${NRF51822_SRC_PATH}/*.cpp
          ${NRF51822_SRC_PATH}/btle/*.cpp
          ${NRF51822_SRC_PATH}/btle/custom/*.cpp
          ${NRF51822_SRC_PATH}/nordic/*.cpp
          ${NRF51822_SRC_PATH}/nordic/app_common/*.cpp
          ${NRF51822_SRC_PATH}/nordic/ble/*.cpp
          ${NRF51822_SRC_PATH}/nordic/ble/ble_services/*.cpp
          ${NRF51822_SRC_PATH}/nordic/bootloader_dfu/*.c
    )
add_sources(${NRF51822_SOURCES})

add_sources(${MBED_SRC_PATH}/targets/cmsis/TARGET_${MBED_VENDOR}/TARGET_${MBED_FAMILY}/${TOOLCHAIN}/startup_NRF51822.s)

############################################################################
# By now, we've traversed all subdirectories and have collected everything that
# needs to be built. We can define the build targets.
############################################################################
# add MbedTest as a build target depending on all the sources
add_executable(${MAIN_TARGET} ${SRCS})

# Add a post-build dependency like printing size of the
# resulting binary and copying to the target.
add_custom_command(
    TARGET ${MAIN_TARGET}
    COMMAND ${SIZE_COMMAND} ${MAIN_TARGET}
    COMMAND ${OBJCOPY_COMMAND} ${MAIN_TARGET} -O binary ${PROJECT_NAME}.bin  # convert .elf to .bin
    COMMAND ${OBJCOPY_COMMAND} ${MAIN_TARGET} -O ihex ${PROJECT_NAME}.hex  # convert .elf to .hex
    #COMMAND srec_cat ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s110_nrf51822_7_1_0/s110_nrf51822_7.1.0_softdevice.hex -intel ${PROJECT_NAME}.bin -binary -offset 0x16000 -o combined.hex -intel
    COMMAND mergehex -m ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s110_nrf51822_7_1_0/s110_nrf51822_7.1.0_softdevice.hex ${PROJECT_NAME}.hex -o combined.hex
    # follow this by copying the resulting combined.hex onto the target (possibly over USB)
)
