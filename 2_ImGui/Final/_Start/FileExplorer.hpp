#pragma once

#include <filesystem>
#include <string_view>

#include <imgui.h>
#include <implot.h>

namespace fs = std::filesystem;

class FileExplorer
{
public:
    FileExplorer()
        : currentPath(fs::current_path()), selectedEntry(fs::path{}){};
    virtual ~FileExplorer(){};

    void Draw(std::string_view label);

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
