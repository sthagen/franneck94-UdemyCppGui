#pragma once

#include <string>
#include <string_view>

class WindowClass
{
public:
    static constexpr auto bufferSize = 1024;

public:
    WindowClass() : textBuffer({}), currentFilename({})
    {
        textBuffer.reserve(bufferSize);
    }

    void Draw(std::string_view label);
    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
    std::string textBuffer;
    std::string currentFilename;

    constexpr static auto popUpFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto popUpSize = ImVec2(300.0F, 100.0F);
};

void render(WindowClass &window_class);
