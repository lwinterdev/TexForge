#include "InputManager.h"

// ========================================
// Constructor
// ========================================

InputManager::InputManager()
    : mouseX(0.0),
    mouseY(0.0)
{
}


// ========================================
// Update
// ========================================

void InputManager::Update(GLFWwindow* window)
{
    glfwGetCursorPos(
        window,
        &mouseX,
        &mouseY
    );
}

void InputManager::UpdateMousePosition(GLFWwindow* window)
{
    glfwGetCursorPos(
        window,
        &mouseX,
        &mouseY
    );
}

// ========================================
// Mouse position
// ========================================

double InputManager::GetMouseX() const
{
    return mouseX;
}


double InputManager::GetMouseY() const
{
    return mouseY;
}


// ========================================
// Mouse button
// ========================================

bool InputManager::IsMouseButtonPressed(
    GLFWwindow* window,
    int button
) const
{
    return glfwGetMouseButton(
        window,
        button
    ) == GLFW_PRESS;
}


// ========================================
// Camera
// ========================================

void InputManager::ProcessCameraInput(
    GLFWwindow* window,
    Camera& camera
)
{
    camera.Inputs(window);
}