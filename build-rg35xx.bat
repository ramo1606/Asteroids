:: build-rg35xx-vs.bat
:: Use this if you have Visual Studio installed
@echo off
echo Building for RG35XX Plus using Visual Studio NMake...

:: Setup Visual Studio environment for NMake
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 (
    call "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvars64.bat"
    if errorlevel 1 (
        call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvars64.bat"
        if errorlevel 1 (
            echo ERROR: Could not find Visual Studio 2022. Please update the path above.
            pause
            exit /b 1
        )
    )
)

if not exist build-rg35xx mkdir build-rg35xx
cd build-rg35xx

cmake .. -G "NMake Makefiles" ^
    -DCMAKE_TOOLCHAIN_FILE=../rg35xx-toolchain.cmake ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_FOR_RG35XX=ON
if errorlevel 1 goto error

nmake
if errorlevel 1 goto error

echo.
echo Build successful! Executable is in build-rg35xx\rg35xx\
echo Copy the executable to your RG35XX Plus
goto end

:error
echo Build failed!

:end
cd ..
pause