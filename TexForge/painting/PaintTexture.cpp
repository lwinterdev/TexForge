#include "PaintTexture.h"

#include <algorithm>


// ============================================================
// Constructor - new blank texture
// ============================================================

PaintTexture::PaintTexture(
    int width,
    int height)
{
    this->width = width;
    this->height = height;

    CreateBlackPixels();

    CreateGPUTexture();

    Upload();
}


// ============================================================
// Constructor - existing OpenGL texture
// ============================================================

PaintTexture::PaintTexture(
    int width,
    int height,
    GLuint existingTextureID)
{
    this->width = width;
    this->height = height;

    ID = existingTextureID;

    // Create a CPU-side buffer.
    //
    // We initialize it to black because we don't
    // automatically know what is currently inside
    // the existing GPU texture.
    CreateBlackPixels();
}


// ============================================================
// Create black CPU texture
// ============================================================

void PaintTexture::CreateBlackPixels()
{
    pixels.resize(
        static_cast<size_t>(width) *
        static_cast<size_t>(height) *
        4
    );

    // RGB = 0
    // A   = 255
    //
    // Result:
    //
    // (0, 0, 0, 255)
    //
    // completely black and opaque.

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            size_t index =
                (static_cast<size_t>(y) *
                    static_cast<size_t>(width) +
                    static_cast<size_t>(x)) * 4;

            pixels[index + 0] = 0;   // R
            pixels[index + 1] = 0;   // G
            pixels[index + 2] = 0;   // B
            pixels[index + 3] = 255; // A
        }
    }
}


// ============================================================
// Create OpenGL texture
// ============================================================

void PaintTexture::CreateGPUTexture()
{
    glGenTextures(
        1,
        &ID
    );

    glBindTexture(
        GL_TEXTURE_2D,
        ID
    );


    // Texture filtering

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


    // Texture wrapping

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


    // Allocate the texture.

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );


    // Unbind.

    glBindTexture(
        GL_TEXTURE_2D,
        0
    );
}


// ============================================================
// Upload CPU texture to GPU
// ============================================================

void PaintTexture::Upload()
{
    if (ID == 0)
        return;


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


// ============================================================
// Set pixel
// ============================================================

void PaintTexture::SetPixel(
    int x,
    int y,
    const glm::vec4& color)
{
    // Ignore pixels outside texture.

    if (x < 0 ||
        x >= width ||
        y < 0 ||
        y >= height)
    {
        return;
    }


    size_t index =
        (static_cast<size_t>(y) *
            static_cast<size_t>(width) +
            static_cast<size_t>(x)) * 4;


    pixels[index + 0] =
        static_cast<unsigned char>(
            std::clamp(color.r, 0.0f, 1.0f) * 255.0f
            );

    pixels[index + 1] =
        static_cast<unsigned char>(
            std::clamp(color.g, 0.0f, 1.0f) * 255.0f
            );

    pixels[index + 2] =
        static_cast<unsigned char>(
            std::clamp(color.b, 0.0f, 1.0f) * 255.0f
            );

    pixels[index + 3] =
        static_cast<unsigned char>(
            std::clamp(color.a, 0.0f, 1.0f) * 255.0f
            );
}


// ============================================================
// Get pixel
// ============================================================

glm::vec4 PaintTexture::GetPixel(
    int x,
    int y) const
{
    if (x < 0 ||
        x >= width ||
        y < 0 ||
        y >= height)
    {
        return glm::vec4(0.0f);
    }


    size_t index =
        (static_cast<size_t>(y) *
            static_cast<size_t>(width) +
            static_cast<size_t>(x)) * 4;


    return glm::vec4(
        pixels[index + 0] / 255.0f,
        pixels[index + 1] / 255.0f,
        pixels[index + 2] / 255.0f,
        pixels[index + 3] / 255.0f
    );
}


// ============================================================
// Get pixel buffer
// ============================================================

std::vector<unsigned char>&
PaintTexture::GetPixels()
{
    return pixels;
}


const std::vector<unsigned char>&
PaintTexture::GetPixels() const
{
    return pixels;
}