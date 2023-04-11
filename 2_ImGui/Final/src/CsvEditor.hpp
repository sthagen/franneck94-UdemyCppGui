#pragma once

#include <string_view>
#include <vector>

#include "imgui.h"
#include "implot.h"

#include "BaseView.hpp"

class CsvEditor : public BaseView
{
public:
    virtual ~CsvEditor(){};

    void Draw(std::string_view label, bool *open = nullptr) override;

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
