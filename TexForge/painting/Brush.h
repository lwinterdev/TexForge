#pragma once

#include <glm/glm.hpp>

class PaintTexture;


class Brush
{
public:

    float radius = 25.0f;

    float opacity = 1.0f;

    glm::vec4 color =
        glm::vec4(
            1.0f,
            0.0f,
            0.0f,
            1.0f
        );


    void Paint(
        PaintTexture& texture,
        int centerX,
        int centerY
    );
};