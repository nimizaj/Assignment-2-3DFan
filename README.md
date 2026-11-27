# Cross-Platform 3D Interactive Fan (C++, OpenGL ES 3.0, WebGL)

A real-time 3D interactive fan application compiled from a single C++ codebase for **Windows Desktop**, **Android (NDK)**, and **WebGL (Emscripten)**, with automated GitHub Actions CI/CD deployment to GitHub Pages.

**Live Browser Demo:** https://nimizaj.github.io/Assignment-2-3DFan/

---

## Problem & Constraint
Deliver consistent 3D rendering and interactive gesture/camera controls across desktop, mobile, and web browsers from one shared C++ codebase while staying within WebGL browser memory and shader restrictions.

## Design Choice & Trade-off
- **Chosen:** A unified OpenGL ES 3.0 rendering pipeline and CMake build configuration shared across Windows, Android, and Emscripten rather than maintaining separate desktop OpenGL extensions.
- **Trade-off:** Gave up desktop-only shader features in exchange for zero visual divergence across platforms and simpler cross-platform maintenance.

## What Was Tested, What Failed & What Changed
- **Tested:** Cross-platform builds, gesture logging (`Fan3D` on Android Logcat), shader compilation, and frame pacing across Windows, Android NDK, and WebGL.
- **What failed:** Desktop GLSL shaders and synchronous main-loop behavior failed strict GLSL ES 3.00 precision checks and browser event-loop limits when compiled with Emscripten.
- **What changed:** Standardized explicit GLSL ES 3.00 precision qualifiers, adapted the render loop for Emscripten WebGL, and added a GitHub Actions workflow to verify and deploy WebGL builds automatically on every push to `main`, sustaining 60 FPS across all 3 platforms.

---

## Build Instructions

### 1. Windows Desktop
**Prerequisites:** Visual Studio or C++ Build Tools installed.
- **To Run:** Double-click `script_build_and_run.bat` in the root directory.
- **Output:** Downloads dependencies (`GLFW`, `GLEW`, `GLM`), compiles via CMake, and opens a desktop window rendering the 3D fan. Press `ESC` to quit.

### 2. Android
**Prerequisites:** Android Studio and Android NDK installed.
- **To Run:** Open the `android/` folder in Android Studio, wait for Gradle to sync, select a device/emulator, and click **Run**.
- **Output:** Installs and launches on the device. View gestures and logs in Android Studio Logcat filtered by `Fan3D`.

### 3. WebGL (Emscripten)
**Prerequisites:** Emscripten SDK (`emsdk`) installed and activated in your PATH, plus Python for local hosting.
- **To Run:**
  1. From the project root, run `./build_web.sh` (Mac/Linux/WSL) or `build_web.bat` (Windows).
  2. Navigate into the output folder: `cd web`
  3. Start a local server: `python -m http.server 8080`
- **Output:** Open `http://localhost:8080` in your browser to run the WebGL build locally. Every push to `main` also triggers the GitHub Actions CI/CD pipeline to automatically rebuild and deploy the live demo to GitHub Pages.
