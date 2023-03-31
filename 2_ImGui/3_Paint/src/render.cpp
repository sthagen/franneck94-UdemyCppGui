#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"


void DrawingTool::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    if (ImGui::Button("Save"))
    {
        SaveToFile("drawing.bmp");
    }

    ImGui::Text("Canvas:");
    const auto canvas_pos = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton("canvas", canvas_size);
    const auto mouse_pos = ImGui::GetMousePos();
    const auto is_mouse_hovering = ImGui::IsItemHovered();

    if (is_mouse_hovering && ImGui::IsMouseDown(0))
    {
        const auto point =
            ImVec2(mouse_pos.x - canvas_pos.x, mouse_pos.y - canvas_pos.y);
        points.push_back(point);
    }

    const auto *draw_list = ImGui::GetWindowDrawList();
    for (const auto &point : points)
    {
        draw_list->AddCircleFilled(
            ImVec2(canvas_pos.x + point.x, canvas_pos.y + point.y),
            2.0f,
            ImColor(255, 255, 255),
            16);
    }

    ImGui::End();
}

void DrawingTool::SaveToFile(std::string_view filename)
{
    const auto canvas_pos = ImGui::GetCursorScreenPos();

    auto *draw_list = ImGui::GetWindowDrawList();
    for (const auto &point : points)
    {
        const auto draw_pos =
            ImVec2(canvas_pos.x + point.x, canvas_pos.y + point.y);
        const auto rounded_pos =
            ImVec2(static_cast<int>(draw_pos.x), static_cast<int>(draw_pos.y));
        AddPixel(rounded_pos, ImColor(255, 255, 255));
    }

    WriteBMP(filename);
}

void DrawingTool::AddPixel(const ImVec2 &pos, const ImColor &color)
{
    const auto x = static_cast<int>(pos.x);
    const auto y = static_cast<int>(pos.y);

    if (x >= 0 && x < canvas_size.x && y >= 0 && y < canvas_size.y)
    {
        const auto offset =
            static_cast<int>((canvas_size.y - y - 1) * canvas_size.x + x) * 4;

        image_buffer[offset] = static_cast<uint8_t>(color.Value.x);
        image_buffer[offset + 1] = static_cast<uint8_t>(color.Value.y);
        image_buffer[offset + 2] = static_cast<uint8_t>(color.Value.z);
        image_buffer[offset + 3] = 255; // opacity
    }
}

void DrawingTool::WriteBMP(std::string_view filename)
{
    auto file = std::ofstream(filename.data(), std::ios::out | std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Could not open the file for writing!\n";
        return;
    }

    const auto width = static_cast<int32_t>(canvas_size.x);
    const auto height = static_cast<int32_t>(canvas_size.y);
    const auto row_size = ((width * 3 + 3) / 4) * 4;
    const auto bmp_size = 54 + (row_size * height);

    // BMP header
    uint8_t header[54] = {
        'B', 'M',                   // Signature
        0,   0,   0, 0,             // File size
        0,   0,   0, 0,             // Reserved
        54,  0,   0, 0,             // Offset to pixel data
        40,  0,   0, 0,             // DIB header size
        0,   0,   0, 0,             // Width
        0,   0,   0, 0,             // Height
        1,   0,                     // Color planes
        24,  0,                     // Bits per pixel
        0,   0,   0, 0,             // Compression method
        0,   0,   0, 0,             // Image size
        0,   0,   0, 0, 0, 0, 0, 0, // Resolution (dpi)
        0,   0,   0, 0, 0, 0, 0, 0, // Number of colors and important colors
    };

    memcpy(header + 2, &bmp_size, 4);
    memcpy(header + 18, &width, 4);
    memcpy(header + 22, &height, 4);

    file.write(reinterpret_cast<char *>(header), 54);

    // Write the image data
    for (int32_t y = height - 1; y >= 0; --y)
    {
        for (int32_t x = 0; x < width; ++x)
        {
            int32_t offset = (y * width + x) * 4;
            file.write(reinterpret_cast<char *>(&image_buffer[offset]), 3);
        }
        // Padding for 4-byte alignment
        for (int32_t pad = 0; pad < row_size - width * 3; ++pad)
        {
            file.put(0);
        }
    }

    file.close();
}

void render(DrawingTool &drawingTool)
{
    drawingTool.Draw("Drawing Editor");
}
