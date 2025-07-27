@echo off
echo Building Ice Climber Clone with CMake and MinGW...

if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

echo.
if exist IceClimberClone.exe (
    echo ✅ Build successful! Executable created: IceClimberClone.exe
) else (
    echo ❌ Build failed!
)
