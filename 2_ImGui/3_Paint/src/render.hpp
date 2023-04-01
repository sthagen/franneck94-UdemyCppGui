#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include <imgui.h>

class DrawingTool
{
public:
    DrawingTool()
        : canvas_size(800, 600),
          image_buffer(canvas_size.x * canvas_size.y * 4, 0){};
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
};


void render(DrawingTool &drawingTool);
