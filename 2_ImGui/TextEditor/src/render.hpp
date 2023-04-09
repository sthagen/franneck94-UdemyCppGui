#pragma once

#include <string>
#include <string_view>

class WindowClass
{
public:
    static constexpr auto bufferSize = 1024;

public:
    WindowClass() : currentFilename({})
    {
        textBuffer.reserve(bufferSize);
    }

    void Draw(std::string_view title);
    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
    std::string textBuffer;
    std::string currentFilename;
};

void render(WindowClass &window_class);
