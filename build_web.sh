#!/usr/bin/env sh
set -e

mkdir -p web

# Fetch header-only GLM if not already cached
GLM_DIR="/tmp/glm"
if [ ! -d "$GLM_DIR/glm" ]; then
    git clone --depth 1 https://github.com/g-truc/glm.git "$GLM_DIR"
fi

em++ -std=c++17 \
    Scene/main.cpp \
    Scene/Fan.cpp \
    Scene/Renderer.cpp \
    Scene/Transform.cpp \
    -IScene \
    -I"$GLM_DIR" \
    -DUSE_GLFW \
    -s USE_GLFW=3 \
    -s USE_WEBGL2=1 \
    -s FULL_ES3=1 \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file android/app/src/main/assets/shader@assets/shader \
    -o web/index.html

echo ""
echo "Build succeeded."
echo "Run:  cd web && python3 -m http.server 8080"
echo "Open: http://localhost:8080/"
