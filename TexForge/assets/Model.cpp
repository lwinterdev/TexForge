#include "Model.h"

Model::Model(const std::string& path)
{
    LoadModel(path);
}

void Model::Draw(Shader& shader)
{
    for (Mesh& mesh : meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(
    const std::string& path)
{
}