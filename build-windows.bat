:: build-windows.bat
@echo off
echo Building for Windows Development...

if not exist build-windows mkdir build-windows
cd build-windows

cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto error

cmake --build . --config Release
if errorlevel 1 goto error

echo.
echo Build successful! Executable is in build-windows\Release\
goto end

:error
echo Build failed!

:end
cd ..
pause