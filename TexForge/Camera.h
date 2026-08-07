#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"

class Camera
{
public:

    // Position and orientation
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Window size
    int width;
    int height;

    // Camera movement
    float speed = 0.1f;
    float sensitivity = 100.0f;

    // Projection settings
    float FOV = 45.0f;
    float NearPlane = 0.1f;
    float FarPlane = 100.0f;

    // Camera matrices
    glm::mat4 view;
    glm::mat4 projection;

    bool firstClick = true;

    Camera(
        int width,
        int height,
        glm::vec3 position
    );

    // Updates view/projection matrices and uploads them
    void Matrix(
        Shader& shader,
        const char* uniform
    );

    // Camera movement
    void Inputs(
        GLFWwindow* window
    );

    // Matrix accessors
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetCameraMatrix() const;
};

#endif