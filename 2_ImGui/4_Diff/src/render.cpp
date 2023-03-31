#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

void FileDiffTool::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    ImGui::InputText("File 1", &file1Path);
    ImGui::InputText("File 2", &file2Path);

    if (ImGui::Button("Compare"))
    {
        file1Content = LoadFileContent(file1Path);
        file2Content = LoadFileContent(file2Path);
        CreateDiff();
    }

    ImGui::BeginChild("Diff",
                      ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, -1.0f),
                      true);
    ImGui::Text("File 1:");
    for (std::size_t i = 0; i < diffResult1.size(); ++i)
    {
        if (!diffResult1[i].empty() || !diffResult2[i].empty())
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                               "%s",
                               diffResult1[i].c_str());
        }
        else
        {
            ImGui::Text("%s", diffResult1[i].c_str());
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Diff2",
                      ImVec2(ImGui::GetContentRegionAvail().x, -1.0f),
                      true);
    ImGui::Text("File 2:");

    for (std::size_t i = 0; i < diffResult2.size(); ++i)
    {
        if (!diffResult1[i].empty() || !diffResult2[i].empty())
        {
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                               "%s",
                               diffResult2[i].c_str());
        }
        else
        {
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
                               "%s",
                               diffResult2[i].c_str());
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

std::vector<std::string> FileDiffTool::LoadFileContent(
    std::string_view filePath)
{
    auto content = std::vector<std::string>{};
    auto file = std::ifstream(filePath.data());

    if (file.is_open())
    {
        auto line = std::string;
        while (std::getline(file, line))
        {
            content.push_back(line);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filePath.data() << std::endl;
    }

    return content;
}

void FileDiffTool::CreateDiff()
{
    diffResult1.clear();
    diffResult2.clear();

    const auto maxLines = std::max(file1Content.size(), file2Content.size());
    for (std::size_t i = 0; i < maxLines; ++i)
    {
        const auto line1 = i < file1Content.size() ? file1Content[i] : "";
        const auto line2 = i < file2Content.size() ? file2Content[i] : "";

        if (line1 != line2)
        {
            diffResult1.push_back(line1);
            diffResult2.push_back(line2);
        }
        else
        {
            diffResult1.push_back("");
            diffResult2.push_back("");
        }
    }
}

void render(FileDiffTool &fileDiffTool)
{
    fileDiffTool.Draw("File Diff Tool");
}
