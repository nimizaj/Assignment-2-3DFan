@echo off
REM =============================================================================
REM build_web.bat – Build TouchEvents-Modern for WebGL using Emscripten (Windows)
REM =============================================================================

IF NOT EXIST web MKDIR web

REM --- GLFW backend (default) -----------------------------------------------
emcc -std=c++17 ^
     Scene/main.cpp ^
     Scene/Renderer.cpp ^
     Scene/Fan.cpp ^
     Scene/Transform.cpp ^
     -IScene ^
     -DUSE_GLFW ^
     -s USE_GLFW=3 ^
     -s USE_WEBGL2=1 ^
     -s FULL_ES3=1 ^
     -s WASM=1 ^
     -s ALLOW_MEMORY_GROWTH=1 ^
     --preload-file android/app/src/main/assets/shader@assets/shader ^
     -o web/index.html

IF %ERRORLEVEL% NEQ 0 (
    echo Build FAILED.
    pause
    exit /b 1
)

echo.
echo Build succeeded.
echo Run:  cd web ^&^& python -m http.server 8080
echo Open: http://localhost:8080/
pause