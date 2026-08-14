#pragma once

#include <memory>
#include <filesystem>

#include <imgui.h>
#include "../imfilebrowser.h"
#include "./ModelImporter.h"
#include "./Model.h"


// ========================================
// Model Manager
// ========================================

class ModelManager
{
public:

    // ----------------------------------------
    // Constructor
    // ----------------------------------------

    ModelManager();


    // ----------------------------------------
    // Dialog
    // ----------------------------------------

    void OpenDialog();

    void ProcessDialog();


    // ----------------------------------------
    // Current model
    // ----------------------------------------

    Model* GetModel();

    const Model* GetModel() const;


private:

    // ----------------------------------------
    // Model loading
    // ----------------------------------------

    void LoadModel(
        const std::filesystem::path& path
    );


    // ----------------------------------------
    // File dialog
    // ----------------------------------------

    ImGui::FileBrowser modelDialog;


    // ----------------------------------------
    // Model importer
    // ----------------------------------------

    ModelImporter importer;


    // ----------------------------------------
    // Current model
    // ----------------------------------------

    std::unique_ptr<Model> model;
};