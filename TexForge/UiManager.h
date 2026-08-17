#pragma once

// ========================================
// GLFW
// ========================================

#include <GLFW/glfw3.h>


// ========================================
// Project
// ========================================

#include "painting/Brush.h"
#include "engine/RayPicker.h"


// Forward declaration
class ModelManager;


// ========================================
// UI Manager
// ========================================

class UIManager
{
public:

    // ====================================
    // Constructor
    // ====================================

    UIManager(
        Brush& brush,
        RaycastHit& hit,
        ModelManager& modelManager,
        double& mouseX,
        double& mouseY
    );


    // ====================================
    // Initialization
    // ====================================

    bool Initialize(
        GLFWwindow* window
    );


    // ====================================
    // Rendering
    // ====================================

    void Render();


    // ====================================
    // Shutdown
    // ====================================

    void Shutdown();


private:

    // ====================================
    // Window
    // ====================================

    GLFWwindow* window = nullptr;


    // ====================================
    // References to application data
    // ====================================

    Brush& brush;

    RaycastHit& hit;

    ModelManager& modelManager;

    double& mouseX;
    double& mouseY;
};