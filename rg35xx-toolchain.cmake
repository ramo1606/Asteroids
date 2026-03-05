# Toolchain file for RG35XX Plus (ARM Cortex-A53 - ARM64/AArch64)
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=rg35xx-toolchain.cmake -DBUILD_FOR_RG35XX=ON ..

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Specify the cross-compiler
# You need to download AArch64 GNU Toolchain for Windows
# Download from: https://developer.arm.com/downloads/-/gnu-a
# Choose: arm-gnu-toolchain-XX.X-mingw-w64-i686-aarch64-none-linux-gnu.tar.xz
# Or if you're using Linaro toolchain: https://releases.linaro.org/components/toolchain/binaries/

# Set the path to your AArch64 toolchain (adjust this path to match your installation)
set(TOOLCHAIN_PATH "C:/Program Files (x86)/Arm GNU Toolchain aarch64-none-linux-gnu/14.3 rel1/bin")

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-gcc.exe)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-g++.exe)
set(CMAKE_AR ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-ar.exe)
set(CMAKE_RANLIB ${TOOLCHAIN_PATH}/aarch64-none-linux-gnu-ranlib.exe)

# Target specific flags for Cortex-A53 (ARM64)
set(CMAKE_C_FLAGS "-march=armv8-a -mtune=cortex-a53 -O2 -ffunction-sections -fdata-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")

# Where to look for target environment
set(CMAKE_FIND_ROOT_PATH ${CMAKE_CURRENT_LIST_DIR}/rg35xx-sysroot)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Specify the path where RG35XX libraries are located
# You'll need to copy SDL2, EGL, GLESv2 .so files from your RG35XX
set(RG35XX_LIBS_PATH "${CMAKE_CURRENT_LIST_DIR}/rg35xx-sysroot/usr/lib" CACHE PATH "Path to RG35XX libraries")