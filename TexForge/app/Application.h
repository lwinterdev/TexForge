#pragma once

#include <memory>
#include <filesystem>

#include <GLFW/glfw3.h>

#include "../shaderClass.h"
#include "../Camera.h"
#include "../assets/Model.h"
#include "../assets/ModelImporter.h"
#include "../painting/PaintTexture.h"
#include "../painting/Brush.h"
#include "../engine/RayPicker.h"

#include <imgui.h>
#include "../imfilebrowser.h"


class Application
{
public:

    Application();
    ~Application();


    bool Initialize();
    void Run();
    void Shutdown();


private:

    // ========================================
    // Initialization
    // ========================================

    bool InitializeGLFW();
    bool InitializeOpenGL();
    bool InitializeImGui();


    // ========================================
    // Main loop
    // ========================================

    void ProcessInput();
    void Update();
    void Render();
    void RenderUI();


    // ========================================
    // Painting
    // ========================================

    void ProcessPainting();


    // ========================================
    // Model
    // ========================================

    void OpenModelDialog();
    void ProcessModelDialog();

    void LoadModel(
        const std::filesystem::path& path
    );


private:

    // ========================================
    // Window
    // ========================================

    GLFWwindow* window = nullptr;

    int windowWidth = 800;
    int windowHeight = 800;


    // ========================================
    // Rendering
    // ========================================

    std::unique_ptr<Shader> shaderProgram;

    std::unique_ptr<Camera> camera;


    // ========================================
    // Model
    // ========================================

    ModelImporter importer;

    std::unique_ptr<Model> model;


    // ========================================
    // Painting
    // ========================================

    std::unique_ptr<PaintTexture> paintTexture;

    Brush brush;


    // ========================================
    // File dialog
    // ========================================

    ImGui::FileBrowser modelDialog;


    // ========================================
    // Raycast
    // ========================================

    RaycastHit hit;


    // ========================================
    // Mouse
    // ========================================

    double mouseX = 0.0;
    double mouseY = 0.0;
};