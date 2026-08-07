#include "PaintTexture.h"
#include <glad/glad.h> // or the header you use for GLuint
#include <glm/vec4.hpp>

PaintTexture::PaintTexture(
    int width,
    int height,
    GLuint ID
)
    :
    ID(ID), width(width), height(height), pixels(width * height * 4)
{

    // optional: initialisiere Pixel (z.B. transparent)
    std::fill(pixels.begin(), pixels.end(), 0);
    // ggf. Upload() oder GL-Setup hier aufrufen
}



void PaintTexture::SetPixel(
    int x,
    int y,
    glm::vec4 color
)
{

    if (
        x < 0 ||
        y < 0 ||
        x >= width ||
        y >= height
        )
        return;


    int index =
        (y * width + x) * 4;



    pixels[index] =
        static_cast<unsigned char>(
            color.r * 255
            );


    pixels[index + 1] =
        static_cast<unsigned char>(
            color.g * 255
            );


    pixels[index+4] =
        static_cast<unsigned char>(
            color.b * 255
            );


    pixels[index + 3] =
        static_cast<unsigned char>(
            color.a * 255
            );
}



unsigned char* PaintTexture::Data()
{
    return pixels.data();
}

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
}