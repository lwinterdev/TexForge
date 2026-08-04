#pragma once

#include <vector>
#include <string>

#include "Mesh.h"

class Model
{
public:

    // All meshes that make up this model
    std::vector<Mesh> meshes;

    // Optional: original file path
    std::string path;

    // Constructors
    Model() = default;

    explicit Model(const std::string& path);

    // Draw the whole model
    void Draw(Shader& shader);

private:

    // Used later by Assimp
    void LoadModel(const std::string& path);
};