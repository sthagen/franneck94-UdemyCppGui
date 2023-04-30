#pragma once

#include <filesystem>
#include <string_view>

#include <imgui.h>

namespace fs = std::filesystem;

class WindowClass
{
public:
    WindowClass()
        : currentPath(fs::current_path()), selectedEntry(fs::path{}){};

    void Draw(std::string_view label);
    void DrawMenu();
    void DrawContent();
    void DrawActions();
    void DrawFilter();

private:
    void openFileWithDefaultEditor(const fs::path &filePath);
    bool renameFile(const fs::path &oldPath, const fs::path &newPath);
    bool deleteFile(const fs::path &path);

private:
    fs::path currentPath;
    fs::path selectedEntry;
    bool renameDialogOpen = false;
    bool deleteDialogOpen = false;
};

void render(WindowClass &window_class);
