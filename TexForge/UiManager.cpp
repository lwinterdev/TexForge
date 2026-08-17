#include "UIManager.h"

#include "painting/Brush.h"
#include "assets/Model.h"
#include "FileDialog.h"
#include "engine/RayPicker.h"
#include "assets/ModelManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>


// ========================================
// Constructor
// ========================================

UIManager::UIManager(
    Brush& brush,
    RaycastHit& hit,
    ModelManager& modelManager,
    double& mouseX,
    double& mouseY
)
    : brush(brush),
    hit(hit),
    modelManager(modelManager),
    mouseX(mouseX),
    mouseY(mouseY)
{
}


// ========================================
// Initialize
// ========================================

bool UIManager::Initialize(
    GLFWwindow* window
)
{
    this->window = window;


    // ----------------------------
    // ImGui version
    // ----------------------------

    IMGUI_CHECKVERSION();


    // ----------------------------
    // Create context
    // ----------------------------

    ImGui::CreateContext();


    // ----------------------------
    // IO
    // ----------------------------

    ImGuiIO& io =
        ImGui::GetIO();

    (void)io;


    // ----------------------------
    // Theme
    // ----------------------------

    ImGui::StyleColorsDark();


    // ----------------------------
    // GLFW backend
    // ----------------------------

    if (!ImGui_ImplGlfw_InitForOpenGL(
        window,
        true))
    {
        std::cout
            << "Failed to initialize ImGui GLFW backend\n";

        return false;
    }


    // ----------------------------
    // OpenGL backend
    // ----------------------------

    if (!ImGui_ImplOpenGL3_Init(
        "#version 330"))
    {
        std::cout
            << "Failed to initialize ImGui OpenGL backend\n";

        return false;
    }


    return true;
}


// ========================================
// Render
// ========================================

void UIManager::Render()
{
    // ----------------------------
    // Start ImGui frame
    // ----------------------------

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();


    // ====================================
    // Main window
    // ====================================

    ImGui::Begin(
        "TexForge"
    );


    // ----------------------------
    // FPS
    // ----------------------------

    ImGuiIO& io =
        ImGui::GetIO();


    ImGui::Text(
        "FPS: %.1f",
        io.Framerate
    );


    ImGui::Separator();


    // ====================================
    // Brush
    // ====================================

    ImGui::Text(
        "Brush"
    );


    ImGui::SliderFloat(
        "Brush Size",
        &brush.radius,
        1.0f,
        100.0f
    );


    ImGui::ColorEdit4(
        "Brush Color",
        &brush.color.x
    );


    ImGui::Separator();


    // ====================================
    // Selection
    // ====================================

    ImGui::Text(
        "Selection"
    );


    if (hit.hit)
    {
        ImGui::Text(
            "Surface hit"
        );


        ImGui::Text(
            "UV: %.3f, %.3f",
            hit.uv.x,
            hit.uv.y
        );


        ImGui::Text(
            "Position: %.3f, %.3f, %.3f",
            hit.position.x,
            hit.position.y,
            hit.position.z
        );
    }
    else
    {
        ImGui::Text(
            "No surface hit"
        );
    }


    ImGui::Separator();


    // ====================================
    // Model
    // ====================================

    if (ImGui::Button(
        "Open Model"
    ))
    {
        modelManager.OpenDialog();
    }


    if (modelManager.GetModel())
    {
        ImGui::Text(
            "Model loaded"
        );
    }
    else
    {
        ImGui::Text(
            "No model loaded"
        );
    }


    ImGui::Separator();


    // ====================================
    // Save
    // ====================================

    if (ImGui::Button(
        "Save Project"
    ))
    {
        std::cout
            << "Saving...\n";
    }


    ImGui::End();

    // ====================================
    // Brush cursor
    // ====================================

    ImDrawList* drawList =
        ImGui::GetForegroundDrawList();


    drawList->AddCircle(
        ImVec2(
            static_cast<float>(mouseX),
            static_cast<float>(mouseY)
        ),
        brush.radius,
        IM_COL32(
            255,
            255,
            255,
            255
        )
    );


    // ====================================
    // Render ImGui
    // ====================================

    ImGui::Render();


    ImGui_ImplOpenGL3_RenderDrawData(
        ImGui::GetDrawData()
    );
}


// ========================================
// Shutdown
// ========================================

void UIManager::Shutdown()
{
    if (!window)
        return;


    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();


    window = nullptr;
}