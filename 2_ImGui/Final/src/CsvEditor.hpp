#pragma once

#include <string_view>
#include <vector>

#include <imgui.h>
#include <implot.h>

#include "WindowBase.hpp"

class CsvEditor : public WindowBase
{
public:
    CsvEditor(){};
    virtual ~CsvEditor(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void DrawSizeButtons();
    void DrawIoButtons();
    void DrawTable();

    void SaveToFile(std::string_view filename);
    void LoadFromFile(std::string_view filename);

    template <typename T>
    void PlotCellValue(std::string_view formatter, const T value);

private:
    std::int32_t numCols = 0;
    std::int32_t numRows = 0;
    std::vector<std::vector<float>> data;
};
