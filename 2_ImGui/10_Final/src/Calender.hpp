#pragma once

#include <string>
#include <string_view>

#include "BaseView.hpp"

class Calender : public BaseView
{
public:
    static constexpr auto bufferSize = 1024;

public:
    Calender() : currentFilename({})
    {
        textBuffer.reserve(bufferSize);
    }
    virtual ~Calender() = default;

    void Draw(std::string_view title, bool *open = NULL) final;

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    std::string GetFileExtension(std::string_view filename);

private:
    std::string textBuffer;
    std::string currentFilename;
};
