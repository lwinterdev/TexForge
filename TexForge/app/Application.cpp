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
// Constructor
// ========================================

Application::Application()
    : ui(
        brush,
        hit,
        model,
        modelDialog,
        mouseX,
        mouseY
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


    // ----------------------------
    // Configure file dialog
    // ----------------------------

    modelDialog.SetTitle(
        "Open Asset"
    );


    modelDialog.SetTypeFilters({
        ".obj",
        ".fbx",
        ".gltf",
        ".glb"
        });
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
    // Shader
    // ----------------------------

    shaderProgram =
        std::make_unique<Shader>(
            "vertex.vert",
            "fragment.frag"
        );


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

    glfwGetCursorPos(
        window,
        &mouseX,
        &mouseY
    );


    // ----------------------------
    // Raycast
    // ----------------------------

    hit = RaycastHit();


    if (model && camera)
    {
        Ray ray =
            RayPicker::ScreenPointToRay(
                mouseX,
                mouseY,
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

    ProcessPainting();


    // ----------------------------
    // File dialog
    // ----------------------------

    ProcessModelDialog();
}


// ========================================
// Painting
// ========================================

void Application::ProcessPainting()
{
    if (!model)
        return;


    if (!paintTexture)
        return;


    if (!hit.hit)
        return;


    if (
        glfwGetMouseButton(
            window,
            GLFW_MOUSE_BUTTON_RIGHT
        ) != GLFW_PRESS
        )
    {
        return;
    }


    // ----------------------------
    // UV -> texture coordinates
    // ----------------------------

    int x =
        static_cast<int>(
            hit.uv.x *
            paintTexture->width
            );


    int y =
        static_cast<int>(
            (1.0f - hit.uv.y) *
            paintTexture->height
            );


    // ----------------------------
    // Paint
    // ----------------------------

    brush.Paint(
        *paintTexture,
        x,
        y
    );


    // ----------------------------
    // Upload to GPU
    // ----------------------------

    paintTexture->Upload();
}


// ========================================
// Rendering
// ========================================

void Application::Render()
{
    // ----------------------------
    // Clear
    // ----------------------------

    glClearColor(
        0.07f,
        0.13f,
        0.17f,
        1.0f
    );


    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_DEPTH_BUFFER_BIT
    );


    if (!shaderProgram)
        return;


    // ----------------------------
    // Activate shader
    // ----------------------------

    shaderProgram->Activate();


    // ----------------------------
    // Camera matrices
    // ----------------------------

    if (camera)
    {
        camera->Matrix(
            *shaderProgram,
            "camMatrix"
        );
    }


    // ----------------------------
    // Paint texture
    // ----------------------------

    if (paintTexture)
    {
        glActiveTexture(
            GL_TEXTURE0
        );


        glBindTexture(
            GL_TEXTURE_2D,
            paintTexture->ID
        );


        glUniform1i(
            glGetUniformLocation(
                shaderProgram->ID,
                "tex0"
            ),
            0
        );
    }


    // ----------------------------
    // Model
    // ----------------------------

    if (model)
    {
        model->Draw(
            *shaderProgram
        );
    }
}


// ========================================
// Open model dialog
// ========================================

void Application::OpenModelDialog()
{
    modelDialog.Open();
}


// ========================================
// Process model dialog
// ========================================

void Application::ProcessModelDialog()
{
    if (!modelDialog.HasSelected())
        return;


    std::filesystem::path path =
        modelDialog.GetSelected();


    std::cout
        << "Importing model: "
        << path.string()
        << "\n";


    LoadModel(path);


    modelDialog.ClearSelected();
}


// ========================================
// Load model
// ========================================

void Application::LoadModel(
    const std::filesystem::path& path)
{
    try
    {
        // ----------------------------
        // Import model
        // ----------------------------

        Model importedModel =
            importer.Load(
                path.string()
            );


        // ----------------------------
        // Replace current model
        // ----------------------------

        model =
            std::make_unique<Model>(
                std::move(importedModel)
            );


        std::cout
            << "Model imported successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "Failed to import model:\n"
            << e.what()
            << "\n";
    }
    catch (...)
    {
        std::cout
            << "Failed to import model.\n";
    }
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
    // Shader
    // ----------------------------

    if (shaderProgram)
    {
        shaderProgram->Delete();

        shaderProgram.reset();
    }


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