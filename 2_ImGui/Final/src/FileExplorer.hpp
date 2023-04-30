#pragma once

#include <filesystem>
#include <string_view>

#include <imgui.h>
#include <implot.h>

#include "WindowBase.hpp"

namespace fs = std::filesystem;

class FileExplorer : public WindowBase
{
public:
    FileExplorer()
        : currentPath(fs::current_path()), selectedEntry(fs::path{}){};
    virtual ~FileExplorer(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

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
