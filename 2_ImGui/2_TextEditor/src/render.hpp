#pragma once

#include <cstdint>
#include <string_view>

class WindowClass
{
public:
    static const std::size_t BUFFER_SIZE = 1024;

public:
    WindowClass()
    {
        textBuffer.reserve(BUFFER_SIZE);
    };

    void Draw(std::string_view title);
    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);

    std::string GetFileExtension(std::string_view filePath);

    bool visible;

private:
    std::string textBuffer;
};


void render(WindowClass &window_class);
