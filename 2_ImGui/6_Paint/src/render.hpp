#pragma once

#include <cstdint>
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
        : canvas_size(ImVec2(static_cast<float>(num_rows),
                             static_cast<float>(num_cols))),
          canvas_pos(ImVec2{}){};
    void Draw(std::string_view title);

private:
    void SaveToImageFile(std::string_view filename);
    void LoadFromImageFile(std::string_view filename);

    void AddPixel(const ImVec2 &pos,
                  const ImColor &color,
                  std::vector<uint8_t> &buffer);

    void DrawColorButtons();

    void ClearCanvas();

private:
    const std::uint32_t num_rows = 800;
    const std::uint32_t num_cols = 600;
    const std::uint32_t num_channels = 3;
    ImVec2 canvas_size;

    std::vector<PointData> points;

    ImVec2 canvas_pos;

    ImColor current_draw_color = ImColor(255, 255, 255);
    float point_draw_size = 2.0f;

    char filename_buffer[128] = "test.bin";

    float border_thickness = 1.5f;
};

void render(WindowClass &window_class);
