#include "FileDialog.h"


// ========================================
// Constructor
// ========================================

FileDialog::FileDialog()
{
}


// ========================================
// Configuration
// ========================================

void FileDialog::SetTitle(
    const std::string& title)
{
    fileBrowser.SetTitle(
        title
    );
}


void FileDialog::SetTypeFilters(
    const std::vector<std::string>& filters)
{
    fileBrowser.SetTypeFilters(
        filters
    );
}


// ========================================
// Dialog
// ========================================

void FileDialog::Open()
{
    fileBrowser.Open();
}


void FileDialog::Display()
{
    fileBrowser.Display();
}


// ========================================
// Selection
// ========================================

bool FileDialog::HasSelected() const
{
    return fileBrowser.HasSelected();
}


std::filesystem::path FileDialog::GetSelected() const
{
    return fileBrowser.GetSelected();
}


void FileDialog::ClearSelected()
{
    fileBrowser.ClearSelected();
}