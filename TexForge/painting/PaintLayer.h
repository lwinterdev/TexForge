#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class PaintLayer
{
public:

    std::string name;

    int width;
    int height;

    bool visible = true;
    float opacity = 1.0f;

    // RGBA pixels
    std::vector<unsigned char> pixels;

    PaintLayer(
        const std::string& name,
        int width,
        int height,
        glm::vec4 color
    );

    void Clear(glm::vec4 color);

    void SetPixel(
        int x,
        int y,
        glm::vec4 color
    );
};
