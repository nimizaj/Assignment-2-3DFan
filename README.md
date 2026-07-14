# 3D Fan Assignment

This repository contains a cross-platform 3D fan built in C++ using OpenGL ES 3.0.

## Build Instructions

### 1. Windows Desktop
**Prerequisites:** Visual Studio or C++ Build Tools installed.
* **To Run:** Double-click the `script_build_and_run.bat` file in the root directory. 
* **Output:** It will download dependencies (GLFW, GLEW, GLM), compile, and immediately open a desktop window rendering the fan. Press `ESC` to quit.

### 2. Android
**Prerequisites:** Android Studio and Android NDK installed.
* **To Run:** Open the `android/` folder in Android Studio. Wait for Gradle to sync, select your device/emulator, and click the green "Run" (Play) button.
* **Output:** The app will install and launch on your device. Gestures and logs can be viewed in Android Studio's Logcat filtering for `Fan3D`.

### 3. WebGL (Emscripten)
**Prerequisites:** Emscripten SDK (`emsdk`) installed and activated in your terminal path. Python installed for local hosting.
* **To Run:** 1. Open a terminal at the project root and run `./build_web.sh` (Mac/Linux/WSL) or `build_web.bat` (Windows).
  2. Navigate into the output folder: `cd web`
  3. Start a local server: `python -m http.server 8080`
* **Output:** Open your browser to `http://localhost:8080` to view the fan running in WebGL. 
*(Note: A CI/CD pipeline is also configured to automatically build and deploy this target to GitHub Pages on every push to `main`; however, live hosting is pending approval of the GitHub Student Developer Pack).*