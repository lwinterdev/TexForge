#pragma once

#include <memory>
#include <filesystem>


// ========================================
// GLFW
// ========================================

#include <GLFW/glfw3.h>


// ========================================
// Project
// ========================================

#include "../ShaderClass.h"
#include "../Camera.h"
#include "../painting/Brush.h"
#include "../painting/PaintTexture.h"
#include "../engine/RayPicker.h"
#include "../assets/ModelManager.h"
#include "../UiManager.h"
#include "../Input/InputManager.h"


// ========================================
// Application
// ========================================

class Application
{
public:

    // ====================================
    // Constructor / Destructor
    // ====================================

    Application();
    ~Application();


    // ====================================
    // Main
    // ====================================

    bool Initialize();
    void Run();
    void Shutdown();


private:

    // ====================================
    // Initialization
    // ====================================

    bool InitializeGLFW();
    bool InitializeOpenGL();


    // ====================================
    // Main loop
    // ====================================

    void ProcessInput();
    void Update();
    void Render();


    // ====================================
    // Painting
    // ====================================

    void ProcessPainting();


    // ====================================
    // Model
    // ====================================

    void ProcessModelDialog();
    void OpenModelDialog();


    // ====================================
    // Window
    // ====================================

    GLFWwindow* window = nullptr;

    int windowWidth = 1280;
    int windowHeight = 720;


    // ====================================
    // Input
    // ====================================

	InputManager input;

    // ====================================
    // Rendering
    // ====================================

    std::unique_ptr<Shader> shaderProgram;
    std::unique_ptr<Camera> camera;


    // ====================================
    // Texture
    // ====================================

    std::unique_ptr<PaintTexture> paintTexture;


    // ====================================
    // Painting
    // ====================================

    Brush brush;


    // ====================================
    // Raycasting
    // ====================================

    RaycastHit hit;


    // ====================================
    // Model
    // ====================================

    ModelManager modelManager;


    // ====================================
    // UI
    // ====================================

    UIManager ui;
};