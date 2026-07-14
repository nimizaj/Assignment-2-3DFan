// LOG_TAG must be defined before Platform.h is included
#define LOG_TAG "Fan3D"

#include "Fan.h"
#include "Platform.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

Fan::Fan() : movedDistance(0.0f), lastX(0.0f), lastY(0.0f) {
    modelType = CubeType;
}

Fan::~Fan() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

void Fan::InitModel() {
    // 1. Define the 8-vertex cube positions (unit cube centered at origin)
    const float positions[24] = {
        // Front face (Z = +0.5)
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Back face (Z = -0.5)
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    // 2. Define the shade gradient per vertex (Front = 1.0, Back = 0.6)
    const float shades[8] = {
        1.0f, 1.0f, 1.0f, 1.0f, // Front corners
        0.6f, 0.6f, 0.6f, 0.6f  // Back corners
    };

    // 3. Define the 36 indices to draw the 12 triangles of the cube
    const unsigned short indices[36] = {
        0, 1, 2, 2, 3, 0, // Front
        1, 5, 6, 6, 2, 1, // Right
        5, 4, 7, 7, 6, 5, // Back
        4, 0, 3, 3, 7, 4, // Left
        3, 2, 6, 6, 7, 3, // Top
        4, 5, 1, 1, 0, 4  // Bottom
    };

    // Generate OpenGL buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    // Bind VAO
    glBindVertexArray(vao);

    // Setup VBO using a single buffer with sub-regions (Part 1 Requirement)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(shades), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(shades), shades);

    // Attribute 0: Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Attribute 1: Shades (Starts after the positions data)
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, (void*)sizeof(positions));
    glEnableVertexAttribArray(1);

    // Setup IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);

    // Retrieve uniform locations dynamically from the bound shader program
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    mvpLocation = glGetUniformLocation(program, "MODELVIEWPROJECTIONMATRIX");
    partColorLocation = glGetUniformLocation(program, "PARTCOLOR");
}

void Fan::Render() {
    // --- Part 2: Animate the blades ---
    if (fanOn) {
        spinAngle += (kBaseSpeed + dragBoost);
        if (spinAngle >= 360.0f) {
            spinAngle -= 360.0f; // Wrap to prevent overflow
        }
    }

    // --- Part 1: Hierarchical Matrix Stack ---
    transform.TransformSetMatrixMode(MODEL_MATRIX);
    transform.TransformLoadIdentity();

    // World View (Pull back and tilt)
    transform.TransformTranslate(0.0f, -0.8f, -8.0f);
    transform.TransformRotate(glm::radians(20.0f), 0.0f, 1.0f, 0.0f);

    glBindVertexArray(vao);
    glm::mat4 mvp;

    // 1. Base (Brown)
    transform.TransformPushMatrix();
    transform.TransformTranslate(0.0f, -2.6f, 0.0f);
    transform.TransformScale(1.6f, 0.25f, 0.8f);
    transform.TransformGetModelViewProjectionMatrix(&mvp);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(partColorLocation, 0.45f, 0.28f, 0.12f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
    transform.TransformPopMatrix();

    // 2. Pole (Light Gray)
    transform.TransformPushMatrix();
    transform.TransformTranslate(0.0f, -1.21f, 0.0f);
    transform.TransformScale(0.15f, 2.53f, 0.15f);
    transform.TransformGetModelViewProjectionMatrix(&mvp);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(partColorLocation, 0.55f, 0.55f, 0.58f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
    transform.TransformPopMatrix();

    // 3. Hub (Dark Gray)
    transform.TransformPushMatrix();
    transform.TransformTranslate(0.0f, 0.2f, 0.0f);
    
    transform.TransformPushMatrix(); 
    transform.TransformScale(0.3f, 0.3f, 0.3f);
    transform.TransformGetModelViewProjectionMatrix(&mvp);
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(partColorLocation, 0.20f, 0.20f, 0.22f);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
    transform.TransformPopMatrix(); 

    // 4. Four Blades (Red, Blue, Orange, Green)
    glm::vec3 bladeColors[4] = {
        glm::vec3(1.0f, 0.0f, 0.0f),   // Red
        glm::vec3(0.0f, 0.0f, 1.0f),   // Blue
        glm::vec3(1.0f, 0.5f, 0.0f),   // Orange
        glm::vec3(0.0f, 1.0f, 0.0f)    // Green
    };

    for (int i = 0; i < 4; i++) {
        transform.TransformPushMatrix();
        transform.TransformTranslate(0.0f, 0.2f, 0.15f);
        // Spin logic applies here so only the blades rotate
        transform.TransformRotate(glm::radians(spinAngle + (i * 90.0f)), 0.0f, 0.0f, 1.0f);
        transform.TransformTranslate(0.0f, 0.55f, 0.0f);
        transform.TransformScale(0.22f, 0.8f, 0.05f);
        
        transform.TransformGetModelViewProjectionMatrix(&mvp);
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform3fv(partColorLocation, 1, glm::value_ptr(bladeColors[i]));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
        
        transform.TransformPopMatrix();
    }
    
    transform.TransformPopMatrix(); // Pop the hub's base position

    glBindVertexArray(0);
}

// --- Part 3 & 4: Touch Logic and Swipe Math ---

void Fan::TouchEventDown(float x, float y) {
    lastX = x;
    lastY = y;
    movedDistance = 0.0f;
    lastMoveTime = std::chrono::steady_clock::now();
}

void Fan::TouchEventMove(float x, float y) {
    float dx = x - lastX;
    float dy = y - lastY;
    float dist = std::sqrt(dx * dx + dy * dy);
    movedDistance += dist;

    // Calculate time delta in milliseconds
    auto now = std::chrono::steady_clock::now();
    float dtms = std::chrono::duration<float, std::milli>(now - lastMoveTime).count();

    if (dtms > 0.0f) {
        float velocity = dist / dtms;
        dragBoost = std::min(velocity * kBoostScale, kMaxBoost);
    }

    lastX = x;
    lastY = y;
    lastMoveTime = now;
}

void Fan::TouchEventRelease(float x, float y) {
    // If the finger barely moved, it counts as a tap
    if (movedDistance < kTapThreshold) {
        fanOn = !fanOn;
        LOGI("Fan %s", fanOn ? "ON" : "OFF");
    }
    
    // Always reset the drag boost when letting go
    dragBoost = 0.0f;
}