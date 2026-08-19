#include "PaintingSystem.h"

#include "Brush.h"
#include "PaintTexture.h"
#include "../input/InputManager.h"
#include "../engine/RayPicker.h"

#include <GLFW/glfw3.h>

PaintingSystem::PaintingSystem(
    Brush& brush,
    InputManager& input
)
    : brush(brush),
    input(input)
{
}

void PaintingSystem::Update(
    GLFWwindow* window,
    const RaycastHit& hit,
    PaintTexture* paintTexture
)
{
    if (!paintTexture)
        return;

    if (!hit.hit)
        return;

    if (!input.IsMouseButtonPressed(
        window,
        GLFW_MOUSE_BUTTON_RIGHT
    ))
    {
        return;
    }

    int x =
        static_cast<int>(
            hit.uv.x *
            paintTexture->width
            );

    int y =
        static_cast<int>(
            (1.0f - hit.uv.y) *
            paintTexture->height
            );

    brush.Paint(
        *paintTexture,
        x,
        y
    );

    paintTexture->Upload();
}