#pragma once

#include <glm/glm.hpp>

#include "../Camera.h"
#include "../assets/Model.h"

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct RaycastHit
{
    bool hit = false;

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;

    float distance = 0.0f;
};

class RayPicker
{
public:

    static Ray ScreenPointToRay(
        double mouseX,
        double mouseY,
        int screenWidth,
        int screenHeight,
        const Camera& camera);

    static RaycastHit Raycast(
        const Ray& ray,
        const Model& model);
};