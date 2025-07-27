
@echo off
echo Setting up vcpkg and dependencies for Ice Climber Clone...

REM Check if vcpkg is already installed
where vcpkg >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo ✅ vcpkg already installed.
    goto :install_dependencies
)

echo 📦 Installing vcpkg...

REM Clone vcpkg if it does not exist
if not exist "vcpkg" (
    echo Cloning vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git
    if %ERRORLEVEL% NEQ 0 (
        echo ❌ Error cloning vcpkg. Make sure git is installed.
        pause
        exit /b 1
    )
)

REM Build vcpkg
cd vcpkg
call .\bootstrap-vcpkg.bat
if %ERRORLEVEL% NEQ 0 (
    echo ❌ Error building vcpkg.
    cd ..
    pause
    exit /b 1
)

REM Integrate vcpkg with Visual Studio
.\vcpkg integrate install

cd ..

:install_dependencies
echo 📚 Installing dependencies...

REM Install raylib
vcpkg install raylib

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ✅ Setup completed!
    echo.
    echo Now you can build the project with:
    echo   build_windows.bat
    echo.
    echo Or use CMake directly:
    echo   mkdir build ^&^& cd build
    echo   cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
    echo   cmake --build . --config Release
) else (
    echo ❌ Error installing dependencies.
)

pause
