#pragma once

#include <filesystem>
#include <string_view>

#include <imgui.h>

namespace fs = std::filesystem;

class WindowClass
{
public:
    WindowClass() : currentPath(fs::current_path())
    {
    }

    void Draw(std::string_view label);

private:
    void openFileWithDefaultEditor(const fs::path &filePath);
    bool renameFile(const fs::path &oldPath, const fs::path &newPath);
    bool deleteFile(const fs::path &filePath);

private:
    fs::path currentPath;
    char renameBuffer[1024];

    bool renameDialogOpen = false;
    bool deleteDialogOpen = false;
};

void render(WindowClass &window_class);
