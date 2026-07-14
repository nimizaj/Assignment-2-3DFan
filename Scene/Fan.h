#pragma once
#include "Platform.h"
#include "Model.h"
#include "Transform.h"
#include <chrono>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

class Fan : public Model {
public:
#ifdef PLATFORM_ANDROID
    Fan(AAssetManager* mgr) : assetMgr(mgr), movedDistance(0.0f), lastX(0.0f), lastY(0.0f) { modelType = CubeType; }
    AAssetManager* assetMgr;
#else
    Fan() : movedDistance(0.0f), lastX(0.0f), lastY(0.0f) { modelType = CubeType; }
#endif
    virtual ~Fan();

    virtual void InitModel() override;
    virtual void Render() override;
    virtual void Resize(int w, int h) override; 
    
    virtual void TouchEventDown(float x, float y) override;
    virtual void TouchEventMove(float x, float y) override;
    virtual void TouchEventRelease(float x, float y) override;

private:
    Transform transform;
    float spinAngle = 0.0f;
    bool fanOn = true;
    float dragBoost = 0.0f;
    
    static constexpr float kBaseSpeed = 1.5f;
    static constexpr float kMaxBoost = 20.0f;
    static constexpr float kBoostScale = 8.0f;
    static constexpr float kTapThreshold = 12.0f;
    
    float lastX, lastY, movedDistance;
    std::chrono::steady_clock::time_point lastMoveTime;

    unsigned int vao, vbo, ibo;
    int mvpLocation, partColorLocation;
    
    GLuint programID; 
};