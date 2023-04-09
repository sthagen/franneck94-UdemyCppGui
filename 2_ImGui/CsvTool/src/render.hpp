#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "imgui.h"
#include "implot.h"

class WindowClass
{
public:
    WindowClass() : colNames({}), data({}){};
    void Draw(std::string_view label);

    template <typename T>
    void PlotCellValue(std::string_view formatter, const T &value);

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);
    void SetColNames();

private:
    std::int32_t numCols = 0;
    std::int32_t numRows = 0;
    std::vector<char> colNames;
    std::vector<std::vector<float>> data;
    std::string currentFilename;
};

void render(WindowClass &window_obj);
