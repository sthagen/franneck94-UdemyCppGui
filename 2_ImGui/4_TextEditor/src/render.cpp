#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

namespace fs = std::filesystem;

void WindowClass::Draw(std::string_view title)
{
    static char saveFilenameBuffer[128] = "output.txt";
    static char loadFilenameBuffer[128] = "output.txt";
    static auto currentFileName = std::string{};

    ImGui::Begin(title.data(),
                 NULL,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

    const auto ctrlPressed = ImGui::GetIO().KeyCtrl;
    const auto escPressed =
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape));
    const auto sPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_S));
    const auto lPressed = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L));

    // Display the "Save" button
    if (ImGui::Button("Save"))
    {
        // Open the "Save File" popup
        ImGui::OpenPopup("Save File");
    }

    // Check for Ctrl + S key press
    if (ctrlPressed && sPressed)
    {
        ImGui::OpenPopup("Save File");
    }

    // Display the "Save File" popup
    if (ImGui::BeginPopupModal("Save File",
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Add an input field for the filename
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        // Display the "Save" button inside the popup
        if (ImGui::Button("Save", ImVec2(120, 0)))
        {
            SaveToFile(saveFilenameBuffer);
            currentFileName = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        // Display the "Cancel" button inside the popup
        if (ImGui::Button("Cancel", ImVec2(120, 0)) || escPressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::SameLine();

    // Display the "Load" button
    if (ImGui::Button("Load"))
    {
        // Open the "Load File" popup
        ImGui::OpenPopup("Load File");
    }

    // Check for Ctrl + L key press
    if (ctrlPressed && lPressed)
    {
        ImGui::OpenPopup("Load File");
    }

    // Display the "Load File" popup
    if (ImGui::BeginPopupModal("Load File",
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Add an input field for the filename
        ImGui::InputText("Filename",
                         loadFilenameBuffer,
                         sizeof(loadFilenameBuffer));

        // Display the "Load" button inside the popup
        if (ImGui::Button("Load", ImVec2(120, 0)))
        {
            LoadFromFile(loadFilenameBuffer);
            currentFileName = loadFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        // Display the "Cancel" button inside the popup
        if (ImGui::Button("Cancel", ImVec2(120, 0)) || escPressed)
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Create a child window for the line numbers
    ImGui::BeginChild("LineNumbers",
                      ImVec2(30, -ImGui::GetFrameHeightWithSpacing()),
                      false,
                      ImGuiWindowFlags_NoScrollbar);
    const auto lineCount =
        std::count(textBuffer.begin(), textBuffer.end(), '\n') + 1;
    for (int i = 1; i <= lineCount; ++i)
    {
        ImGui::Text("%d", i);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    const auto flags = (ImGuiInputTextFlags_AllowTabInput |
                        ImGuiInputTextFlags_CtrlEnterForNewLine |
                        ImGuiInputTextFlags_NoHorizontalScroll);

    ImGui::InputTextMultiline("##WindowClass",
                              &textBuffer,
                              ImVec2{1200.0F, 625.0F},
                              flags);

    ImGui::SetCursorPosY(ImGui::GetWindowSize().y -
                         2.0F * ImGui::GetTextLineHeightWithSpacing());

    ImGui::Text("Opened file: %s | File extension: %s",
                currentFileName.c_str(),
                GetFileExtension(currentFileName).c_str());


    ImGui::End();
}

std::string WindowClass::GetFileExtension(std::string_view filePath)
{
    const auto file_path = fs::path(filePath);
    auto file_extension = file_path.extension().string();

    if (!file_extension.empty() && file_extension[0] == '.')
    {
        file_extension.erase(file_extension.begin());
    }
    return file_extension;
}

void WindowClass::SaveToFile(std::string_view filename)
{
    auto outFile = std::ofstream(filename.data());
    if (outFile.is_open())
    {
        outFile << textBuffer;
        outFile.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void WindowClass::LoadFromFile(std::string_view filename)
{
    auto inFile = std::ifstream(filename.data());
    if (inFile.is_open())
    {
        auto buffer = std::stringstream{};
        buffer << inFile.rdbuf();
        textBuffer = buffer.str();
        inFile.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void render(WindowClass &window_class)
{
    window_class.Draw("Text Editor");
}
