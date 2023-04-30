#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

#include <imgui.h>
#include "imgui_stdlib.h"
#include <implot.h>

#include "render.hpp"

namespace fs = std::filesystem;

void WindowClass::Draw(std::string_view label)
{
    static constexpr auto input_flags =
        (ImGuiInputTextFlags_AllowTabInput |
         ImGuiInputTextFlags_NoHorizontalScroll);

    static char saveFilenameBuffer[512] = {"\0"};
    static char loadFilenameBuffer[512] = {"\0"};

    const auto ctrl_pressed = ImGui::GetIO().KeyCtrl;
    const auto esc_pressed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    const auto s_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const auto l_pressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    ImGui::Begin(label.data());

    if (ImGui::Button("Save") || (ctrl_pressed && s_pressed))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load") || (ctrl_pressed && l_pressed))
    {
        ImGui::OpenPopup("Load File");
    }

    if (ImGui::BeginPopupModal("Save File"))
    {
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        if (ImGui::Button("Save", ImVec2(120.0F, 0.0F)))
        {
            SaveToFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120.0F, 0.0F)) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Load File"))
    {
        ImGui::InputText("Filename",
                         loadFilenameBuffer,
                         sizeof(loadFilenameBuffer));

        if (ImGui::Button("Load", ImVec2(120.0F, 0.0F)))
        {
            LoadFromFile(loadFilenameBuffer);
            currentFilename = loadFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120.0F, 0.0F)) || esc_pressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::BeginChild("LineNumbers", ImVec2(30.0F, 625.0F));

    const auto line_count =
        std::count(textBuffer.begin(), textBuffer.end(), '\n') + 1;

    for (auto i = 1; i <= line_count; ++i)
    {
        ImGui::Text("%d", i);
    }

    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::InputTextMultiline("###inputField",
                              &textBuffer,
                              ImVec2(1200.0F, 625.0F),
                              input_flags);

    if (currentFilename.size() > 0)
    {
        const auto file_extension = GetFileExtension(currentFilename);
        ImGui::Text("Opened file %s | File Extension: %s",
                    currentFilename.data(),
                    file_extension.data());
    }
    else
        ImGui::Text("No file opened!");

    ImGui::End();
}

std::string WindowClass::GetFileExtension(std::string_view filename)
{
    const auto file_path = fs::path(filename);
    const auto file_extension = file_path.extension().string();

    return file_extension;
}

void WindowClass::SaveToFile(std::string_view filename)
{
    auto out = std::ofstream(filename.data());

    if (out.is_open())
    {
        out << textBuffer;
        out.close();
    }
}

void WindowClass::LoadFromFile(std::string_view filename)
{
    auto in = std::ifstream(filename.data());

    if (in.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << in.rdbuf();
        textBuffer = buffer.str();
        in.close();
    }
}

void render(WindowClass &window_class)
{
    window_class.Draw("Text Editor");
}
