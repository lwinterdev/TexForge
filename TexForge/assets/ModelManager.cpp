#include "ModelManager.h"

#include <iostream>


// ========================================
// Constructor
// ========================================

ModelManager::ModelManager()
{
    // ----------------------------
    // Configure file dialog
    // ----------------------------

    modelDialog.SetTitle(
        "Open Asset"
    );


    modelDialog.SetTypeFilters({
        ".obj",
        ".fbx",
        ".gltf",
        ".glb"
        });
}


// ========================================
// Open dialog
// ========================================

void ModelManager::OpenDialog()
{
    modelDialog.Open();
}


// ========================================
// Process dialog
// ========================================

void ModelManager::ProcessDialog()
{
    if (!modelDialog.HasSelected())
        return;


    // ----------------------------
    // Get selected path
    // ----------------------------

    std::filesystem::path path =
        modelDialog.GetSelected();


    std::cout
        << "Importing model: "
        << path.string()
        << "\n";


    // ----------------------------
    // Load model
    // ----------------------------

    LoadModel(path);


    // ----------------------------
    // Clear selection
    // ----------------------------

    modelDialog.ClearSelected();
}


// ========================================
// Load model
// ========================================

void ModelManager::LoadModel(
    const std::filesystem::path& path)
{
    try
    {
        // ----------------------------
        // Import model
        // ----------------------------

        Model importedModel =
            importer.Load(
                path.string()
            );


        // ----------------------------
        // Replace current model
        // ----------------------------

        model =
            std::make_unique<Model>(
                std::move(importedModel)
            );


        std::cout
            << "Model imported successfully.\n";
    }
    catch (const std::exception& e)
    {
        std::cout
            << "Failed to import model:\n"
            << e.what()
            << "\n";
    }
    catch (...)
    {
        std::cout
            << "Failed to import model.\n";
    }
}


// ========================================
// Get current model
// ========================================

Model* ModelManager::GetModel()
{
    return model.get();
}


// ========================================
// Get current model (const)
// ========================================

const Model* ModelManager::GetModel() const
{
    return model.get();
}