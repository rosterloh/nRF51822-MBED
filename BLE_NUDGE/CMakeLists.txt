#
# CMake configuration
#
# Please refer to http://www.cmake.org/cmake/help/documentation.html
# You may also refer to http://www.cmake.org/cmake/help/syntax.html for a quick
# introduction to CMake's syntax.

cmake_minimum_required (VERSION 2.8)

# The name of our project is "BLE_NUDGE". CMakeLists files in this project can
# refer to the root source directory of the project as ${BLE_NUDGE_SOURCE_DIR}
# and to the root binary directory of the project as ${BLE_NUDGE_BINARY_DIR}.
project (BLE_NUDGE)

# The version number.
set (BLE_NUDGE_VERSION_MAJOR 1)
set (BLE_NUDGE_VERSION_MINOR 0)

# define some more paths to projects we depend on
set (MBED_SRC_PATH     ${BLE_NUDGE_SOURCE_DIR}/../mbed-src/libraries/mbed)
set (BLE_API_SRC_PATH  ${BLE_NUDGE_SOURCE_DIR}/../BLE_API)
set (NRF51822_SRC_PATH ${BLE_NUDGE_SOURCE_DIR}/../nRF51822)


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
set(TOOLCHAIN_SYSROOT  C:\ProgramData\chocolatey\bin)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_SYSROOT}\arm-none-eabi-g++)
set(CMAKE_C_COMPILER   ${TOOLCHAIN_SYSROOT}\arm-none-eabi-gcc)
set(SIZE_COMMAND       size)
set(MAIN_TARGET        ${PROJECT_NAME}.elf)
SET(CMAKE_CXX_LINK_FLAGS "--libpath=${TOOLCHAIN_SYSROOT}/lib --info=totals --list=.link_totals.txt --scatter ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_ARM_STD/TARGET_MCU_NORDIC_16K/nRF51822.sct")

SET(CMAKE_CXX_LINK_EXECUTABLE
    "armlink <CMAKE_CXX_LINK_FLAGS> <OBJECTS> -o <TARGET>")
enable_language(ASM)

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
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822
    ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_ARM_STD
    ${MBED_SRC_PATH}/targets/hal
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib
    ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/TARGET_NRF51822_MKIT
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

# Generic compiler flags
add_definitions(
    --cpu=Cortex-M0
    --gnu
    -Otime
    --split_sections
    --apcs=interwork
    --brief_diagnostics
    --restrict
    -O3
    --md
    --no_depend_system_headers
    -DTARGET_NRF51822
    -DTARGET_M0
    -DTARGET_NORDIC
    -DTOOLCHAIN_ARM_STD
    -DTOOLCHAIN_ARM
    -D__CORTEX_M0
    -DARM_MATH_CM0
    -DMBED_BUILD_TIMESTAMP=1399904910.34
    -D__MBED__=1
    -DMBED_USERNAME=rosterloh
)

# Language specifc compiler flags.
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} --cpp --no_rtti")
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} --c99")

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
          ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/*.c
          ${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_ARM_STD/sys.cpp
          ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/*.c
          ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/app_common/*.c
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

add_sources(${MBED_SRC_PATH}/targets/cmsis/TARGET_NORDIC/TARGET_MCU_NRF51822/TOOLCHAIN_ARM_STD/TARGET_MCU_NORDIC_16K/startup_nRF51822.s)

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
    COMMAND ${TOOLCHAIN_SYSROOT}/bin/fromelf --i32combined -o ${PROJECT_NAME}.hex ${MAIN_TARGET} # convert .elf to .hex
    COMMAND ${TOOLCHAIN_SYSROOT}/bin/fromelf --bin -o ${PROJECT_NAME}.bin ${MAIN_TARGET} # convert .elf to .bin
    COMMAND srec_cat ${MBED_SRC_PATH}/targets/hal/TARGET_NORDIC/TARGET_MCU_NRF51822/Lib/s110_nrf51822_7_1_0/s110_nrf51822_7.1.0_softdevice.hex -intel ${PROJECT_NAME}.bin -binary -offset 0x16000 -o combined.hex -intel
    # follow this by copying the resulting combined.hex onto the target (possibly over USB)
)
