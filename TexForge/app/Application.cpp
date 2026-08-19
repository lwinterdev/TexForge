#include "Application.h"

#include <iostream>


// ========================================
// OpenGL
// ========================================

#include <glad/glad.h>


// ========================================
// GLM
// ========================================

#include <glm/glm.hpp>


// ========================================
// Global input
// ========================================

InputManager input;


// ========================================
// Constructor
// ========================================

Application::Application()
    : paintingSystem(
        brush,
        input
    ),
    ui(
        brush,
        hit,
        modelManager,
        input
    )
{
    // ----------------------------
    // Default brush
    // ----------------------------

    brush.color =
        glm::vec4(
            1.0f,
            0.0f,
            0.0f,
            1.0f
        );
}


// ========================================
// Destructor
// ========================================

Application::~Application()
{
    Shutdown();
}


// ========================================
// Initialize everything
// ========================================

bool Application::Initialize()
{
    // ----------------------------
    // GLFW
    // ----------------------------

    if (!InitializeGLFW())
        return false;


    // ----------------------------
    // OpenGL / GLAD
    // ----------------------------

    if (!InitializeOpenGL())
        return false;


    // ----------------------------
    // Paint texture
    //
    // IMPORTANT:
    // This MUST happen after
    // the OpenGL context exists.
    // ----------------------------

    paintTexture =
        std::make_unique<PaintTexture>(
            2048,
            2048
        );


    // ----------------------------
    // UI
    // ----------------------------

    if (!ui.Initialize(window))
        return false;


    // ----------------------------
    // Renderer
    // ----------------------------

    if (!renderer.Initialize())
        return false;


    // ----------------------------
    // Camera
    // ----------------------------

    camera =
        std::make_unique<Camera>(
            windowWidth,
            windowHeight,
            glm::vec3(
                0.0f,
                0.0f,
                2.0f
            )
        );


    std::cout
        << "TexForge initialized successfully.\n";


    return true;
}


// ========================================
// GLFW initialization
// ========================================

bool Application::InitializeGLFW()
{
    if (!glfwInit())
    {
        std::cout
            << "Failed to initialize GLFW\n";

        return false;
    }


    // ----------------------------
    // OpenGL version
    // ----------------------------

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );


    // ----------------------------
    // Create window
    // ----------------------------

    window =
        glfwCreateWindow(
            windowWidth,
            windowHeight,
            "TexForge",
            nullptr,
            nullptr
        );


    if (window == nullptr)
    {
        std::cout
            << "Failed to create GLFW window\n";

        glfwTerminate();

        return false;
    }


    glfwMakeContextCurrent(window);


    return true;
}


// ========================================
// OpenGL initialization
// ========================================

bool Application::InitializeOpenGL()
{
    // ----------------------------
    // GLAD
    // ----------------------------

    if (!gladLoadGL())
    {
        std::cout
            << "Failed to initialize GLAD\n";

        glfwDestroyWindow(window);

        window = nullptr;

        glfwTerminate();

        return false;
    }


    // ----------------------------
    // Viewport
    // ----------------------------

    glViewport(
        0,
        0,
        windowWidth,
        windowHeight
    );


    // ----------------------------
    // Depth testing
    // ----------------------------

    glEnable(GL_DEPTH_TEST);


    return true;
}


// ========================================
// Run
// ========================================

void Application::Run()
{
    while (
        window != nullptr &&
        !glfwWindowShouldClose(window)
        )
    {
        // ----------------------------
        // Events
        // ----------------------------

        glfwPollEvents();


        // ----------------------------
        // Input
        // ----------------------------

        ProcessInput();


        // ----------------------------
        // Update
        // ----------------------------

        Update();


        // ----------------------------
        // Rendering
        // ----------------------------

        Render();


        // ----------------------------
        // UI
        // ----------------------------

        ui.Render();


        // ----------------------------
        // Present
        // ----------------------------

        glfwSwapBuffers(window);
    }
}


// ========================================
// Input
// ========================================

void Application::ProcessInput()
{
    if (camera)
    {
        camera->Inputs(window);
    }
}


// ========================================
// Update
// ========================================

void Application::Update()
{
    // ----------------------------
    // Mouse position
    // ----------------------------

    input.UpdateMousePosition(window);


    // ----------------------------
    // Raycast
    // ----------------------------

    hit = RaycastHit();


    Model* model =
        modelManager.GetModel();


    if (model && camera)
    {
        Ray ray =
            RayPicker::ScreenPointToRay(
                input.GetMouseX(),
                input.GetMouseY(),
                windowWidth,
                windowHeight,
                *camera
            );


        hit =
            RayPicker::Raycast(
                ray,
                *model
            );
    }


    // ----------------------------
    // Painting
    // ----------------------------

    paintingSystem.Update(
        window,
        hit,
        paintTexture.get()
    );


    // ----------------------------
    // File dialog
    // ----------------------------

    modelManager.ProcessDialog();
}


// ========================================
// Rendering
// ========================================

void Application::Render()
{
    Model* model =
        modelManager.GetModel();


    renderer.Render(
        camera.get(),
        model,
        paintTexture.get()
    );
}


// ========================================
// Shutdown
// ========================================

void Application::Shutdown()
{
    // Prevent double shutdown
    // if Application is destroyed
    // more than once.

    if (!window)
        return;


    // ----------------------------
    // UI
    // ----------------------------

    ui.Shutdown();


    // ----------------------------
    // Paint texture
    //
    // Must be destroyed BEFORE
    // the OpenGL context/window.
    // ----------------------------

    paintTexture.reset();


    // ----------------------------
    // Renderer
    // ----------------------------

    renderer.Shutdown();


    // ----------------------------
    // OpenGL window
    // ----------------------------

    glfwDestroyWindow(
        window
    );

    window = nullptr;


    // ----------------------------
    // GLFW
    // ----------------------------

    glfwTerminate();
}