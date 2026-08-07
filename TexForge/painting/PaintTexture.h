#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>

class PaintTexture
{

public:

    GLuint ID;

    int width;
    int height;


    std::vector<unsigned char> pixels;


    PaintTexture(
        int width,
        int height,
		GLuint ID
    );


    void SetPixel(
        int x,
        int y,
        glm::vec4 color
    );


    unsigned char* Data();
    void Upload();

};