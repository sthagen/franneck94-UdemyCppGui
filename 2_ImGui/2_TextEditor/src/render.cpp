#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

TextEditor::TextEditor()
{
    textBuffer.reserve(BUFFER_SIZE);
}

void TextEditor::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    if (ImGui::Button("Save"))
    {
        SaveToFile("output.txt");
    }

    if (ImGui::Button("Load"))
    {
        LoadFromFile("output.txt");
    }

    const auto flags = (ImGuiInputTextFlags_AllowTabInput |
                        ImGuiInputTextFlags_CtrlEnterForNewLine |
                        ImGuiInputTextFlags_NoHorizontalScroll);

    ImGui::InputTextMultiline("##TextEditor",
                              &textBuffer,
                              ImVec2{-1.0f, -1.0f},
                              flags);

    ImGui::End();
}

void TextEditor::SaveToFile(std::string_view filename)
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

void TextEditor::LoadFromFile(std::string_view filename)
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

void render(TextEditor &textEditor)
{
    if (textEditor.visible)
    {
        textEditor.Draw("Text Editor");
    }
}
