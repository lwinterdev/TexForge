#include "PaintTexture.h"

#include <algorithm>


// ========================================
// Constructor
// ========================================

PaintTexture::PaintTexture(
    int width,
    int height
)
    : ID(0),
    width(width),
    height(height),
    pixels(
        width*
        height *
        4,
        0
    )
{
    // ----------------------------------------
    // Create OpenGL texture
    // ----------------------------------------

    glGenTextures(
        1,
        &ID
    );


    glBindTexture(
        GL_TEXTURE_2D,
        ID
    );


    // ----------------------------------------
    // Texture parameters
    // ----------------------------------------

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );


    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );


    // ----------------------------------------
    // Upload initial texture
    //
    // pixels are initially black with
    // alpha = 0.
    // ----------------------------------------

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );


    glBindTexture(
        GL_TEXTURE_2D,
        0
    );
}


// ========================================
// Destructor
// ========================================

PaintTexture::~PaintTexture()
{
    if (ID != 0)
    {
        glDeleteTextures(
            1,
            &ID
        );

        ID = 0;
    }
}


// ========================================
// SetPixel
// ========================================

void PaintTexture::SetPixel(
    int x,
    int y,
    const glm::vec4& color
)
{
    if (
        x < 0 ||
        x >= width ||
        y < 0 ||
        y >= height
        )
    {
        return;
    }


    int index =
        (
            y *
            width +
            x
            ) * 4;


    pixels[index + 0] =
        static_cast<unsigned char>(
            std::clamp(
                color.r,
                0.0f,
                1.0f
            ) * 255.0f
            );


    pixels[index + 1] =
        static_cast<unsigned char>(
            std::clamp(
                color.g,
                0.0f,
                1.0f
            ) * 255.0f
            );


    pixels[index + 2] =
        static_cast<unsigned char>(
            std::clamp(
                color.b,
                0.0f,
                1.0f
            ) * 255.0f
            );


    pixels[index + 3] =
        static_cast<unsigned char>(
            std::clamp(
                color.a,
                0.0f,
                1.0f
            ) * 255.0f
            );
}


// ========================================
// BlendPixel
// ========================================

void PaintTexture::BlendPixel(
    int x,
    int y,
    const glm::vec4& color,
    float opacity
)
{
    if (
        x < 0 ||
        x >= width ||
        y < 0 ||
        y >= height
        )
    {
        return;
    }


    opacity =
        std::clamp(
            opacity,
            0.0f,
            1.0f
        );


    int index =
        (
            y *
            width +
            x
            ) * 4;


    float oldR =
        pixels[index + 0] / 255.0f;

    float oldG =
        pixels[index + 1] / 255.0f;

    float oldB =
        pixels[index + 2] / 255.0f;

    float oldA =
        pixels[index + 3] / 255.0f;


    float newR =
        oldR +
        (
            color.r -
            oldR
            ) * opacity;


    float newG =
        oldG +
        (
            color.g -
            oldG
            ) * opacity;


    float newB =
        oldB +
        (
            color.b -
            oldB
            ) * opacity;


    float newA =
        oldA +
        (
            color.a -
            oldA
            ) * opacity;


    pixels[index + 0] =
        static_cast<unsigned char>(
            std::clamp(newR, 0.0f, 1.0f) *
            255.0f
            );


    pixels[index + 1] =
        static_cast<unsigned char>(
            std::clamp(newG, 0.0f, 1.0f) *
            255.0f
            );


    pixels[index + 2] =
        static_cast<unsigned char>(
            std::clamp(newB, 0.0f, 1.0f) *
            255.0f
            );


    pixels[index + 3] =
        static_cast<unsigned char>(
            std::clamp(newA, 0.0f, 1.0f) *
            255.0f
            );
}


// ========================================
// Upload
// ========================================

void PaintTexture::Upload()
{
    glBindTexture(
        GL_TEXTURE_2D,
        ID
    );


    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        width,
        height,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );


    glBindTexture(
        GL_TEXTURE_2D,
        0
    );
}


// ========================================
// GetWidth
// ========================================

int PaintTexture::GetWidth() const
{
    return width;
}


// ========================================
// GetHeight
// ========================================

int PaintTexture::GetHeight() const
{
    return height;
}


// ========================================
// GetID
// ========================================

GLuint PaintTexture::GetID() const
{
    return ID;
}