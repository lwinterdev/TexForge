#include "ModelImporter.h"

#include <glm/glm.hpp>
#include <iostream>

Model ModelImporter::Load(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs
    );

    if (!scene ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        scene->mRootNode == nullptr)
    {
        std::cout << importer.GetErrorString() << std::endl;
        return Model();
    }

    Model model;

    ProcessNode(
        scene->mRootNode,
        scene,
        model
    );

    return model;
}

void ModelImporter::ProcessNode(
    aiNode* node,
    const aiScene* scene,
    Model& model)
{
    // Convert every mesh attached to this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh =
            scene->mMeshes[node->mMeshes[i]];

        model.meshes.push_back(
            ProcessMesh(mesh, scene)
        );
    }

    // Visit every child node
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(
            node->mChildren[i],
            scene,
            model
        );
    }
}

Mesh ModelImporter::ProcessMesh(
    aiMesh* mesh,
    const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // -------------------------
    // Vertices
    // -------------------------

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // Position
        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Normal
        if (mesh->HasNormals())
        {
            vertex.Normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
        else
        {
            vertex.Normal = glm::vec3(0.0f);
        }

        // Texture Coordinates
        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    // -------------------------
    // Indices
    // -------------------------

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Construct your own Mesh
    return Mesh(vertices, indices, std::vector<Texture>());
}