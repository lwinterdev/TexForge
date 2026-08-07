#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"

// Dear ImGui
#include <imgui.h>



// GLM
#include <glm/gtc/type_ptr.hpp>
#include<GLFW/glfw3.h>

Camera::Camera(int width, int height, glm::vec3 position)
    : width(width),
    height(height),
    Position(position),
    view(1.0f),
    projection(1.0f)
{
}

void Camera::Matrix(
    Shader& shader,
    const char* uniform)
{
    // Update camera matrices
    view = glm::lookAt(
        Position,
        Position + Orientation,
        Up
    );

    projection = glm::perspective(
        glm::radians(FOV),
        static_cast<float>(width) / static_cast<float>(height),
        NearPlane,
        FarPlane
    );

    glm::mat4 cameraMatrix =
        projection * view;

    glUniformMatrix4fv(
        glGetUniformLocation(shader.ID, uniform),
        1,
        GL_FALSE,
        glm::value_ptr(cameraMatrix)
    );
}

glm::mat4 Camera::GetViewMatrix() const
{
    return view;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return projection;
}

glm::mat4 Camera::GetCameraMatrix() const
{
    return projection * view;
}

void Camera::Inputs(GLFWwindow* window)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    //----------------------------
    // Keyboard movement
    //----------------------------

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * Orientation;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Position -= speed * Orientation;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Position -= speed * glm::normalize(glm::cross(Orientation, Up));

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Position += speed * glm::normalize(glm::cross(Orientation, Up));

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Position += speed * Up;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        Position -= speed * Up;

    speed =
        (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        ? 0.4f
        : 0.1f;

    //----------------------------
    // Mouse look
    //----------------------------

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick)
        {
            glfwSetCursorPos(window, width / 2.0, height / 2.0);
            firstClick = false;
        }

        double mouseX;
        double mouseY;

        glfwGetCursorPos(
            window,
            &mouseX,
            &mouseY
        );

        float rotX =
            sensitivity *
            static_cast<float>(mouseY - height / 2.0) /
            static_cast<float>(height);

        float rotY =
            sensitivity *
            static_cast<float>(mouseX - width / 2.0) /
            static_cast<float>(width);

        glm::vec3 right =
            glm::normalize(
                glm::cross(
                    Orientation,
                    Up
                )
            );

        glm::vec3 newOrientation =
            glm::rotate(
                Orientation,
                glm::radians(-rotX),
                right
            );

        if (std::abs(glm::dot(newOrientation, Up)) < 0.99f)
        {
            Orientation = newOrientation;
        }

        Orientation =
            glm::rotate(
                Orientation,
                glm::radians(-rotY),
                Up
            );

        Orientation =
            glm::normalize(Orientation);

        glfwSetCursorPos(
            window,
            width / 2.0,
            height / 2.0
        );
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}