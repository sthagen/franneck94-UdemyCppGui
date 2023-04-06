#pragma once

#include <string_view>
#include <tuple>
#include <vector>

#include <imgui.h>

class WindowClass
{
public:
    using PointData = std::tuple<ImVec2, ImColor, float>;

public:
    WindowClass()
        : points({}), canvasPos({}), currentDrawColor(ImColor(255, 255, 255)),
          pointDrawSize(2.0F){};

    void Draw(std::string_view title);

private:
    void SaveToImageFile(std::string_view filename);
    void LoadFromImageFile(std::string_view filename);
    void DrawColorButtons();
    void ClearCanvas();

private:
    const std::uint32_t numRows = 800;
    const std::uint32_t numCols = 600;
    const std::uint32_t channels = 3;
    const ImVec2 canvasSize =
        ImVec2(static_cast<float>(numRows), static_cast<float>(numCols));

    std::vector<PointData> points;
    ImVec2 canvasPos;

    ImColor currentDrawColor;
    float pointDrawSize;

    char filenameBuffer[256] = "test.bin";
};

void render(WindowClass &window_class);
