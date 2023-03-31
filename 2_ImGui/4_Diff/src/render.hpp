#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include <imgui.h>

constexpr auto WINDOW_WIDTH = std::uint32_t{1280};
constexpr auto WINDOW_HEIGHT = std::uint32_t{720};

class FileDiffTool
{
public:
    void Draw(std::string_view title);

private:
    std::vector<std::string> LoadFileContent(std::string_view filePath);
    void CreateDiff();

private:
    std::string file1Path;
    std::string file2Path;
    std::vector<std::string> file1Content;
    std::vector<std::string> file2Content;
    std::vector<std::string> diffResult1;
    std::vector<std::string> diffResult2;
};


void render(FileDiffTool &fileDiffTool);
