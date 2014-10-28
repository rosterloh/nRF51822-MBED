# ------------------------------------------------------------------------------
INCLUDE(CMakeForceCompiler)

#-------------------------------------------------------------------------------
SET(CMAKE_SYSTEM_NAME Generic)

#-------------------------------------------------------------------------------
# specify the cross compiler, later on we will set the correct path
CMAKE_FORCE_C_COMPILER(arm-none-eabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(arm-none-eabi-g++ GNU)

#-------------------------------------------------------------------------------
set(TOOLCHAIN TOOLCHAIN_GCC_ARM)

#-------------------------------------------------------------------------------
# define presets
set(USE_RTOS false)
set(USE_NET false)
set(USE_USB false)
set(USE_DSP false)
