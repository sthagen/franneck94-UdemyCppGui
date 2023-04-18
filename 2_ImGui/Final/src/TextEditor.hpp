#pragma once

#include <string>
#include <string_view>

#include "WindowBase.hpp"

class TextEditor : public WindowBase
{
public:
    static constexpr auto bufferSize = 1024;

public:
    TextEditor() : currentFilename({})
    {
        textBuffer.reserve(bufferSize);
    }
    virtual ~TextEditor(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
    std::string textBuffer;
    std::string currentFilename;
};
