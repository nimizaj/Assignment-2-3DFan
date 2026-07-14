#pragma once
#include "Model.h"
#include "Transform.h"
#include <chrono>

class Fan : public Model {
public:
    Fan();
    virtual ~Fan();

    // Model interface overrides
    virtual void InitModel() override;
    virtual void Render() override;
    
    // Touch event interface overrides
    virtual void TouchEventDown(float x, float y) override;
    virtual void TouchEventMove(float x, float y) override;
    virtual void TouchEventRelease(float x, float y) override;

private:
    Transform transform;
    
    // Core State (Provided by Assignment 2)
    float spinAngle = 0.0f;
    bool fanOn = true;
    float dragBoost = 0.0f;
    
    static constexpr float kBaseSpeed = 1.5f;
    static constexpr float kMaxBoost = 20.0f;
    static constexpr float kBoostScale = 8.0f;
    static constexpr float kTapThreshold = 12.0f;
    
    // Touch tracking state
    float lastX, lastY;
    float movedDistance;
    std::chrono::steady_clock::time_point lastMoveTime;

    // OpenGL Handles
    unsigned int vao, vbo, ibo;
    int mvpLocation, partColorLocation;
};