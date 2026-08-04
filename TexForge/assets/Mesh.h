#pragma once


#include <vector>

#include "Vertex.h"

#include "../Texture.h"

#include "../shaderClass.h"

#include "../VAO.h"
#include "../VBO.h"
#include "..//EBO.h"

class Mesh
{
public:

    // CPU data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // GPU data
    VAO vao;
    VBO vbo;
    EBO ebo;

    Mesh(
        std::vector<Vertex> vertices,
        std::vector<unsigned int> indices,
        std::vector<Texture> textures
    );

    void Draw(Shader& shader);

private:

    void SetupMesh();
};