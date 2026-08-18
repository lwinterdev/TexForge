#include "Renderer.h"

#include <iostream>

#include <glad/glad.h>

#include "../ShaderClass.h"
#include "../Camera.h"
#include "../assets/Model.h"
#include "../painting/PaintTexture.h"


// ========================================
// Destructor
// ========================================

Renderer::~Renderer()
{
    Shutdown();
}


// ========================================
// Initialize
// ========================================

bool Renderer::Initialize()
{
    // ----------------------------
    // Shader
    // ----------------------------

    shaderProgram =
        std::make_unique<Shader>(
            "vertex.vert",
            "fragment.frag"
        );

    if (!shaderProgram)
    {
        std::cout
            << "Failed to create renderer shader\n";

        return false;
    }


    return true;
}


// ========================================
// Render
// ========================================

void Renderer::Render(
    Camera* camera,
    Model* model,
    PaintTexture* paintTexture
)
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


    // ----------------------------
    // Shader
    // ----------------------------

    if (!shaderProgram)
        return;

    shaderProgram->Activate();


    // ----------------------------
    // Camera
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
// Shutdown
// ========================================

void Renderer::Shutdown()
{
    if (shaderProgram)
    {
        shaderProgram->Delete();

        shaderProgram.reset();
    }
}