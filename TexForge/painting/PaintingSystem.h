#pragma once

struct GLFWwindow;

class InputManager;
class Brush;
class PaintTexture;
struct RaycastHit;

class PaintingSystem
{
public:

    PaintingSystem(
        Brush& brush,
        InputManager& input
    );

    void Update(
        GLFWwindow* window,
        const RaycastHit& hit,
        PaintTexture* paintTexture
    );

private:

    Brush& brush;
    InputManager& input;

};