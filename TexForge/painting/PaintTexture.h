#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>


class PaintTexture
{
public:

    // ----------------------------------------
    // Constructor / Destructor
    // ----------------------------------------

    PaintTexture(
        int width,
        int height
    );

    ~PaintTexture();


    // ----------------------------------------
    // Painting
    // ----------------------------------------

    void SetPixel(
        int x,
        int y,
        const glm::vec4& color
    );


    void BlendPixel(
        int x,
        int y,
        const glm::vec4& color,
        float opacity
    );


    // ----------------------------------------
    // GPU
    // ----------------------------------------

    void Upload();


    // ----------------------------------------
    // Access
    // ----------------------------------------

    int GetWidth() const;

    int GetHeight() const;

    GLuint GetID() const;


    // ----------------------------------------
    // Public texture data
    //
    // These are convenient for now while
    // you're developing the painting system.
    // ----------------------------------------

    GLuint ID;

    int width;
    int height;


private:

    // ----------------------------------------
    // CPU pixel data
    // ----------------------------------------

    std::vector<unsigned char> pixels;
};