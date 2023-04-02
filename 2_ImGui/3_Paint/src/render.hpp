#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include <imgui.h>

class WindowClass
{
public:
    WindowClass()
        : canvas_size(800, 600),
          image_buffer(static_cast<int>(canvas_size.x) *
                           static_cast<int>(canvas_size.y) * 4,
                       0){};
    void Draw(std::string_view title);

private:
    void SaveToImageFile(std::string_view filename);
    void LoadFromImageFile(std::string_view filename);

    void ClearCanvas();

    void AddPixel(const ImVec2 &pos, const ImColor &color);
    void WriteImageFile(std::string_view filename);

private:
    ImVec2 canvas_size;
    std::vector<ImVec2> points;
    std::vector<uint8_t> image_buffer;

    bool open_save_popup = false;
    bool open_load_popup = false;
    char filename_buffer[128] = "test.bmp";

    float border_thickness = 1.5f;
};


void render(WindowClass &window_class);
