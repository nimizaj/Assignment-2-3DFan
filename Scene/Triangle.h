#pragma once

/**
 * Triangle.h
 *
 * Renders a 2D triangle with OpenGL ES 3.0 VAO/VBO.
 * Touch/mouse events change vertex colours:
 *   Down    → red
 *   Move    → green
 *   Release → blue (default)
 *
 * Platform-agnostic – shared by Android, Desktop, and WebGL builds.
 * On Android an AAssetManager pointer is stored for shader loading;
 * on other platforms file-system loading is used via ShaderHelper.
 */

#include "Model.h"
#include "Platform.h"

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Triangle : public Model {
public:
#ifdef PLATFORM_ANDROID
    explicit Triangle(AAssetManager* assetMgr);
#else
    Triangle();
#endif
    ~Triangle() override;

    void InitModel()                         override;
    void Render()                            override;

    void TouchEventDown(float x, float y)    override;
    void TouchEventMove(float x, float y)    override;
    void TouchEventRelease(float x, float y) override;

private:
#ifdef PLATFORM_ANDROID
    AAssetManager* mgr = nullptr;
#endif

    GLuint program = 0;
    GLuint vao     = 0;
    GLuint vboPos  = 0;
    GLuint vboCol  = 0;

    // 3 vertices × RGB – updated on touch
    GLfloat colors[9];

    void setAllColors(float r, float g, float b);
    void uploadColors();
};
