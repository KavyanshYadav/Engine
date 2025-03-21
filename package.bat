@echo off
setlocal enabledelayedexpansion

echo Checking for required files...

:: Check for executable
if not exist build\Release\OpenGLRenderer.exe (
    echo ERROR: OpenGLRenderer.exe not found in build\Release directory!
    echo Please build the project first.
    pause
    exit /b 1
)

:: Check for required DLLs
set "MISSING_DLLS="
if not exist libs\glfw3.dll set "MISSING_DLLS=!MISSING_DLLS! glfw3.dll"
:: Add checks for other required DLLs here

if not "!MISSING_DLLS!"=="" (
    echo ERROR: Missing required DLLs:!MISSING_DLLS!
    echo Please ensure all required DLLs are present in the libs directory.
    pause
    exit /b 1
)

:: Check for shader files
if not exist SHADERS\vertexShader.vert set "MISSING_SHADERS=!MISSING_SHADERS! vertexShader.vert"
if not exist SHADERS\fragmentShader.frag set "MISSING_SHADERS=!MISSING_SHADERS! fragmentShader.frag"
if not exist SHADERS\axis.vert set "MISSING_SHADERS=!MISSING_SHADERS! axis.vert"
if not exist SHADERS\axis.frag set "MISSING_SHADERS=!MISSING_SHADERS! axis.frag"

if not "!MISSING_SHADERS!"=="" (
    echo ERROR: Missing required shader files:!MISSING_SHADERS!
    echo Please ensure all shader files are present in the SHADERS directory.
    pause
    exit /b 1
)

echo All required files found. Creating distribution package...

:: Create directories
mkdir dist 2>nul
mkdir dist\bin 2>nul
mkdir dist\SHADERS 2>nul
mkdir dist\config 2>nul

:: Copy executable
echo Copying executable...
copy /Y build\Release\OpenGLRenderer.exe dist\bin\

:: Copy DLLs
echo Copying DLLs...
if exist libs\*.dll copy /Y libs\*.dll dist\bin\
if exist bin\*.dll copy /Y bin\*.dll dist\bin\

:: Copy shader files
echo Copying shader files...
copy /Y SHADERS\*.vert dist\SHADERS\
copy /Y SHADERS\*.frag dist\SHADERS\

:: Create config directory and default config
echo Creating default configuration...
echo [System]> dist\config\engine_config.ini
echo ConfigVersion=1.0>> dist\config\engine_config.ini
echo LastRun=%date%>> dist\config\engine_config.ini

:: Copy documentation
echo Copying documentation...
if exist README.md copy /Y README.md dist\
if exist LICENSE copy /Y LICENSE dist\

:: Create symbolic link
echo Creating symbolic link for SHADERS...
cd dist\bin
mklink /D SHADERS ..\SHADERS
cd ..\..

echo.
echo Distribution package created successfully in 'dist' folder.
echo.
echo NOTE: The SHADERS folder must be present in the same directory as the executable
echo      or in the working directory when running the application.
echo.
echo Ready to create installer.
pause 