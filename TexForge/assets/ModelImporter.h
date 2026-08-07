#pragma once

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"

class ModelImporter
{
public:

    Model Load(const std::string& path);

    std::string lastError;

private:

    void ProcessNode(
        aiNode* node,
        const aiScene* scene,
        Model& model
    );

    Mesh ProcessMesh(
        aiMesh* mesh,
        const aiScene* scene
    );

    std::vector<Texture> LoadMaterialTextures(
        aiMaterial* mat,
        aiTextureType type,
        std::string typeName
    );
};