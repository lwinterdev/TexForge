#include <iostream>

// Dear ImGui core
#include <imgui.h>

// File browser
#include "imfilebrowser.h"

// ImGui integrations
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// OpenGL function loader
#include <glad/glad.h>

// Window and input library
#include <GLFW/glfw3.h>

// Image loading library
#include <stb/stb_image.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// TexForge
#include "shaderClass.h"
#include "Texture.h"
#include "Camera.h"
#include "assets/Model.h"
#include "assets/ModelImporter.h"
#include "painting/PaintTexture.h"
#include "painting/Brush.h"
#include "engine/RayPicker.h"


int main()
{
    // ========================================
    // Initialize GLFW
    // ========================================

    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

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


    // ========================================
    // Create window
    // ========================================

    GLFWwindow* window =
        glfwCreateWindow(
            800,
            800,
            "TexForge",
            nullptr,
            nullptr
        );


    if (window == nullptr)
    {
        std::cout
            << "Failed to create GLFW window\n";

        glfwTerminate();

        return -1;
    }


    glfwMakeContextCurrent(window);


    // ========================================
    // Initialize GLAD
    // ========================================

    if (!gladLoadGL())
    {
        std::cout
            << "Failed to initialize GLAD\n";

        glfwDestroyWindow(window);
        glfwTerminate();

        return -1;
    }


    // ========================================
    // OpenGL setup
    // ========================================

    glViewport(
        0,
        0,
        800,
        800
    );

    glEnable(GL_DEPTH_TEST);


    // ========================================
    // Initialize ImGui
    // ========================================

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io =
        ImGui::GetIO();

    ImGui::StyleColorsDark();


    ImGui_ImplGlfw_InitForOpenGL(
        window,
        true
    );

    ImGui_ImplOpenGL3_Init(
        "#version 330"
    );


    // ========================================
    // Load shader
    // ========================================

    Shader shaderProgram(
        "vertex.vert",
        "fragment.frag"
    );


    // ========================================
    // Load model
    // ========================================

    ModelImporter importer;

    Model model =
        importer.Load(
            "./testcube.obj"
        );


    // ========================================
    // Camera
    // ========================================

    Camera camera(
        800,
        800,
        glm::vec3(
            0.0f,
            0.0f,
            2.0f
        )
    );


    // ========================================
    // Create blank paint texture
    // ========================================

    PaintTexture paintTexture(
        2048,
        2048
    );


    // ========================================
    // Brush
    // ========================================

    Brush brush;

    brush.color =
        glm::vec4(
            1.0f,
            0.0f,
            0.0f,
            1.0f
        );


    // ========================================
    // File dialog
    // ========================================

    ImGui::FileBrowser fileDialog;

    fileDialog.SetTitle(
        "Open Asset"
    );

    fileDialog.SetTypeFilters({
        ".obj",
        ".fbx",
        ".gltf",
        ".glb",
        ".png",
        ".jpg",
        ".jpeg"
        });


    // ========================================
    // Main loop
    // ========================================

    while (!glfwWindowShouldClose(window))
    {
        // ====================================
        // Events
        // ====================================

        glfwPollEvents();


        // ====================================
        // Clear screen
        // ====================================

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


        // ====================================
        // Camera
        // ====================================

        camera.Inputs(window);


        // ====================================
        // Activate shader
        // ====================================

        shaderProgram.Activate();


        camera.Matrix(
            shaderProgram,
            "camMatrix"
        );


        // ====================================
        // Bind paint texture
        // ====================================

        glActiveTexture(
            GL_TEXTURE0
        );

        glBindTexture(
            GL_TEXTURE_2D,
            paintTexture.ID
        );


        glUniform1i(
            glGetUniformLocation(
                shaderProgram.ID,
                "tex0"
            ),
            0
        );


        // ====================================
        // Draw model
        // ====================================

        model.Draw(
            shaderProgram
        );


        // ====================================
        // Get mouse position
        // ====================================

        double mouseX;
        double mouseY;

        glfwGetCursorPos(
            window,
            &mouseX,
            &mouseY
        );


        // ====================================
        // Ray picking
        // ====================================

        Ray ray =
            RayPicker::ScreenPointToRay(
                mouseX,
                mouseY,
                800,
                800,
                camera
            );


        RaycastHit hit =
            RayPicker::Raycast(
                ray,
                model
            );


        // ====================================
        // Painting
        // ====================================

        if (
            glfwGetMouseButton(
                window,
                GLFW_MOUSE_BUTTON_RIGHT
            ) == GLFW_PRESS
            &&
            hit.hit
            )
        {
            // Convert UV coordinates
            // into texture coordinates.

            int x =
                static_cast<int>(
                    hit.uv.x *
                    paintTexture.width
                    );


            int y =
                static_cast<int>(
                    (1.0f - hit.uv.y) *
                    paintTexture.height
                    );


            // Paint into CPU texture.

            brush.Paint(
                paintTexture,
                x,
                y
            );


            // Upload modified pixels
            // to the GPU.

            paintTexture.Upload();
        }


        // ====================================
        // Start ImGui frame
        // ====================================

        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();


        // ====================================
        // Main TexForge window
        // ====================================

        ImGui::Begin(
            "TexForge"
        );


        ImGui::Text(
            "FPS: %.1f",
            io.Framerate
        );


        ImGui::Separator();


        // ====================================
        // Brush settings
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
        // Raycast information
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
        // File browser button
        // ====================================

        if (ImGui::Button(
            "Open File"
        ))
        {
            fileDialog.Open();
        }


        // ====================================
        // Save button
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
        // File dialog
        // ====================================

        fileDialog.Display();


        // ====================================
        // Check file selection
        // ====================================

        if (fileDialog.HasSelected())
        {
            std::cout
                << "Selected filename: "
                << fileDialog
                .GetSelected()
                .string()
                << "\n";


            // For now we just print the
            // selected file.

            // Later this can become:

            // ImportModel(...)
            // LoadTexture(...)
            // OpenProject(...)


            fileDialog.ClearSelected();
        }


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


        // ====================================
        // Present frame
        // ====================================

        glfwSwapBuffers(
            window
        );
    }


    // ========================================
    // Shutdown ImGui
    // ========================================

    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();


    // ========================================
    // Shutdown shader
    // ========================================

    shaderProgram.Delete();


    // ========================================
    // Shutdown GLFW
    // ========================================

    glfwDestroyWindow(
        window
    );

    glfwTerminate();


    return 0;
}