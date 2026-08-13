#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

class PaintTexture
{
public:

    // Create a new blank texture.
    // The texture starts completely black and opaque.
    PaintTexture(
        int width,
        int height
    );

    // Create a PaintTexture from an existing OpenGL texture.
    // Useful while you're transitioning from your current setup.
    PaintTexture(
        int width,
        int height,
        GLuint existingTextureID
    );

    // Upload CPU pixels to the GPU texture.
    void Upload();

    // Set a single pixel.
    void SetPixel(
        int x,
        int y,
        const glm::vec4& color
    );

    // Get a single pixel.
    glm::vec4 GetPixel(
        int x,
        int y
    ) const;

    // Access the CPU pixel buffer.
    std::vector<unsigned char>& GetPixels();

    const std::vector<unsigned char>& GetPixels() const;

    // OpenGL texture ID.
    GLuint ID = 0;

    // Texture dimensions.
    int width = 0;
    int height = 0;

private:

    // CPU-side RGBA pixel data.
    //
    // Four bytes per pixel:
    //
    // R G B A
    //
    std::vector<unsigned char> pixels;

    // Creates the OpenGL texture object.
    void CreateGPUTexture();

    // Initializes the CPU buffer to black.
    void CreateBlackPixels();

    // Prevent accidental copying of GPU resources.
    PaintTexture(const PaintTexture&) = delete;
    PaintTexture& operator=(const PaintTexture&) = delete;
};