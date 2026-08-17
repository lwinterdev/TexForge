#pragma once

#include <imgui.h>

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// Forward declarations
class Model;
class Brush;
class FileDialog;

struct RaycastHit;


class UIManager
{
public:

    // ========================================
    // Constructor
    // ========================================

    UIManager(
        Brush& brush,
        RaycastHit& hit,
        Model*& model,
        FileDialog& modelDialog,
        double& mouseX,
        double& mouseY
    );


    // ========================================
    // Initialization
    // ========================================

    bool Initialize(
        GLFWwindow* window
    );


    // ========================================
    // Rendering
    // ========================================

    void Render();


    // ========================================
    // Shutdown
    // ========================================

    void Shutdown();


private:

    // ========================================
    // UI data
    // ========================================

    Brush& brush;

    RaycastHit& hit;

    Model*& model;

    FileDialog& modelDialog;

    double& mouseX;
    double& mouseY;


    // ========================================
    // Window
    // ========================================

    GLFWwindow* window = nullptr;
};