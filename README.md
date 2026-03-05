# RG35XX Game Development with Raylib 5.5

A cross-platform raylib 5.5 game project that can be compiled for both Windows (development) and RG35XX handheld consoles (Plus/H/40/V) running muOS.

## Target Platform

- **Device**: RG35XX Plus/H/40/V handheld consoles
- **Architecture**: ARM Cortex-A53 (ARMv8-A 64-bit)
- **Operating System**: muOS (Linux-based)
- **Graphics API**: OpenGL ES 2.0/3.0
- **Audio/Input**: SDL2

## Prerequisites

### For Windows Development
- **Visual Studio 2022** with C++ development tools
- CMake 3.15 or newer

### For RG35XX Cross-Compilation
- **Arm GNU Toolchain aarch64-none-linux-gnu**
  - Download from: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
  - Choose: `arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-aarch64-none-linux-gnu.exe`
- RG35XX system libraries and headers (see setup instructions below)

## Project Structure

```
RG35XX/
├── src/                    # Source code files
│   ├── main.c             # Main entry point
│   └── game.c             # Game logic implementation
├── include/               # Header files
│   ├── config.h          # Game configuration and constants
│   └── game.h            # Game function declarations
├── assets/               # Game assets (textures, audio, etc.)
│   └── README.md         # Asset organization guide
├── rg35xx-sysroot/      # RG35XX system libraries and headers
│   └── usr/
│       ├── lib/          # ARM compiled libraries
│       └── include/      # Header files
├── CMakeLists.txt        # CMake configuration
├── build-windows.bat     # Windows build script
├── build-rg35xx.bat      # RG35XX build script
└── rg35xx-toolchain.cmake # RG35XX cross-compilation setup
```

## Setup Instructions

### 1. Extract Libraries from RG35XX Device

Connect to your RG35XX device via SSH and copy the required ARM compiled libraries. From your project folder, navigate to `rg35xx-sysroot/usr/lib` and run:

```bash
# Copy SDL2 libraries
scp root@192.168.8.157:/usr/lib/libSDL* .

# Copy EGL libraries  
scp root@192.168.8.157:/usr/lib/libEGL* .

# Copy OpenGL ES libraries
scp root@192.168.8.157:/usr/lib/libGLES* .
```

**Note**: Replace `192.168.8.157` with your RG35XX device's actual IP address.

### 2. Setup Header Files

Copy the required header files to `rg35xx-sysroot/usr/include/`:

#### SDL2 Headers
- Download SDL2 development headers from https://www.libsdl.org/download-2.0.php
- Or copy from your local SDL2 installation
- Extract/copy all SDL2 header files to `rg35xx-sysroot/usr/include/SDL2/`

#### OpenGL ES Headers
Download and copy the following header directories:
- **GLES2**: OpenGL ES 2.0 headers
- **GLES3**: OpenGL ES 3.0 headers  
- **EGL**: EGL (Embedded Graphics Library) headers
- **KHR**: Khronos platform headers

Place these in `rg35xx-sysroot/usr/include/`

### 3. Install AArch64 Toolchain

1. Download the **Arm GNU Toolchain aarch64-none-linux-gnu** for Windows
2. Install **arm-gnu-toolchain-14.3.rel1-mingw-w64-x86_64-aarch64-none-linux-gnu.exe**

### 4. Update Toolchain Paths

Edit `rg35xx-toolchain.cmake` if your toolchain is in a different location:

```cmake
set(TOOLCHAIN_PATH "C:/your/path/to/aarch64-none-linux-gnu/bin")
```

## Building

### For Windows Development

#### Using Build Script
```batch
build-windows.bat
```

#### Manual Build
```batch
mkdir build-windows
cd build-windows
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

#### Visual Studio Development
1. Open `build-windows/RG35XX_Game.sln` in **Visual Studio 2022**
2. The project is automatically set as the startup project
3. Source files are organized in filters:
   - **Header Files/include/** - All header files
   - **Source Files/src/** - All source files
4. Press F5 to build and run

The executable will be in `build-windows/Release/RG35XX_Game.exe`

### For RG35XX Target

#### Using Build Script
```batch
build-rg35xx.bat
```

#### Manual Build
```batch
mkdir build-rg35xx
cd build-rg35xx
cmake .. -G "NMake" -DCMAKE_TOOLCHAIN_FILE=../rg35xx-toolchain.cmake -DBUILD_FOR_RG35XX=ON
cmake --build .
```

The executable will be in `build-rg35xx/rg35xx/RG35XX_Game`

#### Post-Build Setup
After building for RG35XX, make the binary executable:
```bash
chmod +x RG35XX_Game
```

## Deploying to RG35XX Device

### Transfer the Game
Copy the compiled binary to your RG35XX device:
```bash
scp RG35XX_Game root@192.168.8.157:/mnt/mmc/ports
```

**Note**: Replace `192.168.8.157` with your RG35XX device's actual IP address.

### Run on Device
1. 
2. Navigate to the ports directory:
   /mnt/mmc/ports

3. Execute:
   RG35XX_Game

## Game Features

- **D-PAD Left/Right**: Rotate the square
- **D-PAD Up/Down**: Change colors  
- **A Button**: Scale up
- **B Button**: Scale down
- **START Button**: Exit game

## Code Organization

- `config.h`: Game constants and configuration
- `game.h`: Function declarations and game state structure
- `game.c`: Game logic implementation
- `main.c`: Entry point and main game loop

## Adding New Features

1. Add constants to `include/config.h`
2. Add function declarations to `include/game.h`
3. Implement functions in `src/game.c`
4. Update game state structure as needed
5. Test on both Windows and RG35XX platforms

## Troubleshooting

### Cross-compilation fails
- Verify **Arm GNU Toolchain aarch64-none-linux-gnu** is installed and in PATH
- Check that the compiler name matches: `aarch64-none-linux-gnu-gcc`
- Update compiler paths in `rg35xx-toolchain.cmake`

### Visual Studio can't find raylib
- CMake will automatically download raylib 5.5
- If download fails, check internet connection and Git installation

### Runtime errors on RG35XX
- Verify OpenGL ES 2.0 support is available
- Check that all required .so files are present in `/usr/lib/`

## Customization

### Changing Raylib Version
Edit `CMakeLists.txt`:
```cmake
GIT_TAG 5.5  # Change to desired version
```

### Optimizing for ARM Cortex-A53
The toolchain file includes Cortex-A53 specific optimizations:
- `-march=armv8-a`: ARMv8-A architecture (64-bit ARM)
- `-mtune=cortex-a53`: CPU-specific tuning for RG35XX Plus/H/40/V
- `-O2`: Optimization level (can change to -O3 for more aggressive optimization)

Note: NEON SIMD is automatically included in ARMv8-A architecture.

### Adding Assets
Place assets in the `assets/` directory, and CMake will automatically copy them to the build output.

## Notes

- The RG35XX Plus/H/40/V devices use **muOS** which is Linux-based, so the binary format is ELF
- SDL2 backend is used instead of GLFW for the RG35XX build
- OpenGL ES 2.0/3.0 is used instead of desktop OpenGL
- Target FPS is optimized for handheld gaming experience
- The ARM Cortex-A53 CPU supports 64-bit ARMv8-A architecture
- All RG35XX variants (Plus/H/40/V) share the same ARM Cortex-A53 architecture