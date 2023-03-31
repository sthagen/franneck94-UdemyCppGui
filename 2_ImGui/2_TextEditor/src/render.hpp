#pragma once

#include <cstdint>
#include <string_view>

constexpr auto WINDOW_WIDTH = std::uint32_t{1280};
constexpr auto WINDOW_HEIGHT = std::uint32_t{720};

class TextEditor
{
public:
    const std::size_t BUFFER_SIZE = 1024;

public:
    TextEditor();

    void Draw(std::string_view title);
    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);

    bool visible;

private:
    std::string textBuffer;
};


void render(TextEditor &textEditor);
