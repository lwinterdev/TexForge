#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    // Position in 3D space
    glm::vec3 Position;

    // Surface normal used for lighting
    glm::vec3 Normal;

    // Texture coordinate
    glm::vec2 TexCoords;

    // Vertex color
    glm::vec3 Color;
};