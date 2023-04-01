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
#include <fmt/format.h>

#include "render.hpp"

void FileDiffTool::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    ImGui::InputText("Left", &file1Path);
    ImGui::InputText("Right", &file2Path);

    if (ImGui::Button("Compare"))
    {
        file1Content = LoadFileContent(file1Path);
        file2Content = LoadFileContent(file2Path);
        CreateDiff();
    }

    ImGui::Text(fmt::format("{}", file1Path).c_str());
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        SaveFileContent(file1Path, file1Content);
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.5f + 50.0F);
    ImGui::Text(fmt::format("{}", file2Path).c_str());
    ImGui::SameLine();
    if (ImGui::Button("Save"))
    {
        SaveFileContent(file2Path, file2Content);
    }

    ImGui::BeginChild(
        "Diff",
        ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 50.0F, -1.0f),
        true);
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
            ImGui::Text("%s", file1Content[i].c_str());
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Swap", ImVec2(50.0F, -1.0f), true);
    for (std::size_t i = 0; i < diffResult1.size(); ++i)
    {
        if (!diffResult1[i].empty() || !diffResult2[i].empty())
        {
            if (ImGui::Button("<", ImVec2(10, 18)))
            {
                file1Content[i] = file2Content[i];
                CreateDiff();
            }
            ImGui::SameLine();
            if (ImGui::Button(">", ImVec2(10, 18)))
            {
                file2Content[i] = file1Content[i];
                CreateDiff();
            }
        }
        else
        {
            ImGui::Dummy(ImVec2(0, 10));
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Diff2",
                      ImVec2(ImGui::GetContentRegionAvail().x - 50.0F, -1.0f),
                      true);

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
                               file2Content[i].c_str());
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void FileDiffTool::SaveFileContent(std::string_view filePath,
                                   const std::vector<std::string> &content)
{
    auto file = std::ofstream(filePath.data());

    if (file.is_open())
    {
        for (const auto &line : content)
        {
            file << line << '\n';
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filePath.data() << std::endl;
    }
}

std::vector<std::string> FileDiffTool::LoadFileContent(
    std::string_view filePath)
{
    auto content = std::vector<std::string>{};
    auto file = std::ifstream(filePath.data());

    if (file.is_open())
    {
        auto line = std::string{};
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
