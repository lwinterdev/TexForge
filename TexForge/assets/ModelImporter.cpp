#include "ModelImporter.h"

#include <glm/glm.hpp>
#include <iostream>
#include <filesystem>

#include <assimp/material.h>


Model ModelImporter::Load(const std::string& path)
{    

    if (!std::filesystem::exists(path))
    {
        lastError =
            "File does not exist: " + path;

		std::cout << lastError << std::endl;

        return Model();
    }

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

        std::cout
            << "UV: "
            << vertex.TexCoords.x
            << ", "
            << vertex.TexCoords.y
            << std::endl;

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

    std::vector<Texture> textures;


    // Construct your own Mesh
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material =
            scene->mMaterials[mesh->mMaterialIndex];


        std::vector<Texture> diffuseMaps =
            LoadMaterialTextures(
                material,
                aiTextureType_DIFFUSE,
                "diffuse"
            );


        textures.insert(
            textures.end(),
            diffuseMaps.begin(),
            diffuseMaps.end()
        );
    }

    std::cout
        << "Vertices: "
        << mesh->mNumVertices
        << std::endl;


    return Mesh(
        vertices,
        indices,
        textures
    );
}

std::vector<Texture> ModelImporter::LoadMaterialTextures(
    aiMaterial* mat,
    aiTextureType type,
    std::string typeName)
{
    std::vector<Texture> textures;


    for (unsigned int i = 0;
        i < mat->GetTextureCount(type);
        i++)
    {
        aiString str;


        mat->GetTexture(
            type,
            i,
            &str
        );


        std::cout
            << "Loading texture: "
            << str.C_Str()
            << std::endl;



        Texture texture(
            str.C_Str(),
            GL_TEXTURE_2D,
            GL_TEXTURE0 + textures.size(),
            GL_RGBA,
            GL_UNSIGNED_BYTE
        );


        textures.push_back(std::move(texture));
    }

    std::cout << "Texture count: "
        << mat->GetTextureCount(type)
        << '\n';

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::cout << "Texture path: "
            << str.C_Str()
            << '\n';
    }

    return textures;
}

