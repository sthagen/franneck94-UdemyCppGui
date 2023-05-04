#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto main_window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto main_window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto main_window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(main_window_size);
    ImGui::SetNextWindowPos(main_window_pos);

    ImGui::Begin(label.data(), nullptr, main_window_flags);

    DrawSelection();
    DrawDiffView();
    DrawStats();

    ImGui::End();
}

void WindowClass::DrawSelection()
{
    ImGui::InputText(" Left", &filePath1);
    ImGui::SameLine();
    if (ImGui::Button("Save###Left"))
        SaveFileContent(filePath1, fileContent1);
    ImGui::InputText("Right", &filePath2);
    ImGui::SameLine();
    if (ImGui::Button("Save###Right"))
        SaveFileContent(filePath2, fileContent2);

    if (ImGui::Button("Compare"))
    {
        fileContent1 = LoadFileContent(filePath1);
        fileContent2 = LoadFileContent(filePath2);

        CreateDiff();
    }
}

void WindowClass::DrawDiffView()
{
    const auto parent_size = ImVec2(ImGui::GetContentRegionAvail().x, 500.0F);
    const auto child_size = ImVec2(parent_size.x / 2.0F - 40.0F, parent_size.y);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0F, 0.0F));

    ImGui::BeginChild("Parent", parent_size, true);

    if (ImGui::BeginChild("Diff1", child_size, false))
    {
        for (std::size_t i = 0; i < fileContent1.size(); i++)
        {
            if (!diffResult1[i].empty())
            {
                ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),
                                   "%s",
                                   fileContent1[i].data());
            }
            else
            {
                ImGui::Text("%s", fileContent1[i].data());
            }
        }

        ImGui::EndChild();
    }

    ImGui::SameLine();

    const auto line_heihgt = ImGui::GetTextLineHeightWithSpacing();
    const auto button_size = ImVec2(15.0F, line_heihgt);

    if (ImGui::BeginChild("Swap", ImVec2(80.0F, child_size.y), true))
    {
        for (std::size_t i = 0; i < diffResult1.size(); i++)
        {
            const auto left_label = fmt::format("<##{}", i);
            const auto right_label = fmt::format(">##{}", i);

            if (!diffResult1[i].empty() || !diffResult2[i].empty())
            {
                if (ImGui::Button(left_label.data(), button_size))
                {
                    if (fileContent1.size() > i && fileContent2.size() > i)
                        fileContent1[i] = fileContent2[i];
                    else if (fileContent2.size() > i)
                        fileContent1.push_back(fileContent2[i]);
                    CreateDiff();
                }

                ImGui::SameLine();

                if (ImGui::Button(right_label.data(), button_size))
                {
                    if (fileContent1.size() > i && fileContent2.size() > i)
                        fileContent2[i] = fileContent1[i];
                    else if (fileContent1.size() > i)
                        fileContent2.push_back(fileContent1[i]);
                    CreateDiff();
                }
            }
            else
            {
                const auto cursor_pos = ImGui::GetCursorPosY();
                ImGui::SetCursorPosY(cursor_pos + line_heihgt);
            }
        }

        ImGui::EndChild();
    }

    ImGui::SameLine();

    if (ImGui::BeginChild("Diff2", child_size, false))
    {
        for (std::size_t i = 0; i < fileContent2.size(); i++)
        {
            if (!diffResult2[i].empty())
            {
                ImGui::TextColored(ImVec4(1.0F, 0.0F, 0.0F, 1.0F),
                                   "%s",
                                   fileContent2[i].data());
            }
            else
            {
                ImGui::Text("%s", fileContent2[i].data());
            }
        }

        ImGui::EndChild();
    }

    ImGui::EndChild();

    ImGui::PopStyleVar();
}

void WindowClass::DrawStats()
{
    auto diff_lines_count = 0U;
    for (const auto &line : diffResult1)
    {
        if (!line.empty())
            ++diff_lines_count;
    }

    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetFontSize() -
                         ImGui::GetStyle().FramePadding.y * 5.0F);
    ImGui::Text("Diff lines count: %d", diff_lines_count);
}

WindowClass::FileContent WindowClass::LoadFileContent(
    std::string_view file_path)
{
    auto content = FileContent{};
    auto in = std::ifstream(file_path.data());

    if (in.is_open())
    {
        auto line = std::string{};
        while (std::getline(in, line))
        {
            content.push_back(line);
        }

        in.close();
    }

    return content;
}

void WindowClass::SaveFileContent(std::string_view file_path,
                                  const FileContent &content)
{
    auto out = std::ofstream(file_path.data());

    if (out.is_open())
    {
        for (const auto &line : content)
        {
            out << line << '\n';
        }

        out.close();
    }
}

void WindowClass::CreateDiff()
{
    diffResult1.clear();
    diffResult2.clear();

    const auto max_num_lines =
        std::max(fileContent1.size(), fileContent2.size());

    for (std::size_t i = 0; i < max_num_lines; i++)
    {
        const auto line1 = i < fileContent1.size() ? fileContent1[i] : "EMPTY";
        const auto line2 = i < fileContent2.size() ? fileContent2[i] : "EMPTY";

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

void render(WindowClass &window_class)
{
    window_class.Draw("File Diff Tool");
}
