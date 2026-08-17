#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <imgui.h>
#include "imfilebrowser.h"


// ========================================
// FileDialog
// ========================================
//
// Small wrapper around ImGuiFileBrowser.
//
// Application code only needs to know:
// - configure the dialog
// - open it
// - check for a selection
// - retrieve the selected file
//
// ========================================

class FileDialog
{
public:

    // ====================================
    // Constructor
    // ====================================

    FileDialog();


    // ====================================
    // Configuration
    // ====================================

    void SetTitle(
        const std::string& title
    );

    void SetTypeFilters(
        const std::vector<std::string>& filters
    );


    // ====================================
    // Dialog
    // ====================================

    void Open();

    void Display();


    // ====================================
    // Selection
    // ====================================

    bool HasSelected() const;

    std::filesystem::path GetSelected() const;

    void ClearSelected();


private:

    // ====================================
    // File browser
    // ====================================

    ImGui::FileBrowser fileBrowser;
};