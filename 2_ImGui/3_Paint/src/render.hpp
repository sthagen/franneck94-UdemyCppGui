#pragma once

#include <cstdint>
#include <string_view>
#include <utility>
#include <vector>

#include <imgui.h>

class WindowClass
{
public:
    WindowClass()
        : canvas_size(ImVec2(static_cast<float>(num_rows),
                             static_cast<float>(num_cols))),
          image_buffer(num_rows * num_cols * num_channels, 0),
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
    const std::uint32_t num_channels = 4;
    ImVec2 canvas_size;
    std::vector<uint8_t> image_buffer;

    std::vector<std::pair<ImVec2, ImColor>> points;

    ImVec2 canvas_pos;

    ImColor current_draw_color = ImColor(255, 255, 255);

    bool open_save_popup = false;
    bool open_load_popup = false;
    char filename_buffer[128] = "test.png";

    float border_thickness = 1.5f;
};

void render(WindowClass &window_class);
