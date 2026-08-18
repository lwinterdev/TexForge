#pragma once

#include <GLFW/glfw3.h>
#include <../Camera.h>

class Camera;

class InputManager
{
public:

    InputManager();

    void Update(GLFWwindow* window);
    void UpdateMousePosition(GLFWwindow* window);

    double GetMouseX() const;
    double GetMouseY() const;

    bool IsMouseButtonPressed(
        GLFWwindow* window,
        int button
    ) const;

    void ProcessCameraInput(
        GLFWwindow* window,
        Camera& camera
    );

private:

    double mouseX;
    double mouseY;
};