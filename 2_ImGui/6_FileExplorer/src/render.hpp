#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string_view>
#include <vector>

#include <imgui.h>

constexpr auto WINDOW_WIDTH = std::uint32_t{1280};
constexpr auto WINDOW_HEIGHT = std::uint32_t{720};

namespace fs = std::filesystem;

class FileExplorer
{
public:
    FileExplorer() : currentPath(fs::current_path())
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

void render(FileExplorer &file_explorer);
