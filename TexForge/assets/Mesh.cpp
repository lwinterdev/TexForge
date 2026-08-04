#include "Mesh.h"

#include <utility>

Mesh::Mesh(
    std::vector<Vertex> vertices,
    std::vector<unsigned int> indices,
    std::vector<Texture> textures
)
    :
    vertices(std::move(vertices)),
    indices(std::move(indices)),
    textures(std::move(textures)),
    vbo(this->vertices.data(), this->vertices.size() * sizeof(Vertex)),
    ebo(this->indices.data(), this->indices.size() * sizeof(unsigned int))
{
    SetupMesh();
}

void Mesh::SetupMesh()
{
    vao.Bind();

    vbo = VBO(
        vertices.data(),
        vertices.size() * sizeof(Vertex));

    ebo = EBO(
        indices.data(),
        indices.size() * sizeof(unsigned int));

    vao.LinkAttrib(
        vbo,
        0,
        3,
        GL_FLOAT,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Position));

    vao.LinkAttrib(
        vbo,
        1,
        3,
        GL_FLOAT,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Normal));

    vao.LinkAttrib(
        vbo,
        2,
        2,
        GL_FLOAT,
        sizeof(Vertex),
        (void*)offsetof(Vertex, TexCoords));

    vao.LinkAttrib(
        vbo,
        3,
        3,
        GL_FLOAT,
        sizeof(Vertex),
        (void*)offsetof(Vertex, Color));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Mesh::Draw(Shader& shader)
{
    for (Texture& texture : textures)
    {
        texture.Bind();
    }

    vao.Bind();

    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT,
        nullptr);

    vao.Unbind();
}