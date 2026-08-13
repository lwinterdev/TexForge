#include "PaintLayer.h"

#include <algorithm>

PaintLayer::PaintLayer(
    const std::string& name,
    int width,
    int height,
    glm::vec4 color
)
{
    this->name = name;
    this->width = width;
    this->height = height;

    pixels.resize(
        width * height * 4
    );

    Clear(color);
}


void PaintLayer::Clear(glm::vec4 color)
{
    unsigned char r =
        static_cast<unsigned char>(
            color.r * 255.0f
            );

    unsigned char g =
        static_cast<unsigned char>(
            color.g * 255.0f
            );

    unsigned char b =
        static_cast<unsigned char>(
            color.b * 255.0f
            );

    unsigned char a =
        static_cast<unsigned char>(
            color.a * 255.0f
            );

    for (int i = 0; i < width * height; i++)
    {
        pixels[i * 4 + 0] = r;
        pixels[i * 4 + 1] = g;
        pixels[i * 4 + 2] = b;
        pixels[i * 4 + 3] = a;
    }
}


void PaintLayer::SetPixel(
    int x,
    int y,
    glm::vec4 color
)
{
    if (
        x < 0 ||
        x >= width ||
        y < 0 ||
        y >= height
        )
        return;

    int index =
        (y * width + x) * 4;

    pixels[index + 0] =
        static_cast<unsigned char>(
            color.r * 255.0f
            );

    pixels[index + 1] =
        static_cast<unsigned char>(
            color.g * 255.0f
            );

    pixels[index + 2] =
        static_cast<unsigned char>(
            color.b * 255.0f
            );

    pixels[index + 3] =
        static_cast<unsigned char>(
            color.a * 255.0f
            );
}