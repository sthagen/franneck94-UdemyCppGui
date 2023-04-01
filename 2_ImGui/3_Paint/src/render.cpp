#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "render.hpp"


void DrawingTool::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    const auto canvas_pos = ImGui::GetCursorScreenPos();
    const auto mouse_pos = ImGui::GetMousePos();

    if (ImGui::Button("Save"))
    {
        SaveToImageFile("drawing.bmp");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
        LoadFromImageFile("drawing.bmp");

        points.clear();

        for (int32_t y = 0; y < canvas_size.y; ++y)
        {
            for (int32_t x = 0; x < canvas_size.x; ++x)
            {
                const auto offset =
                    (static_cast<int32_t>(canvas_size.y) - y - 1) *
                        static_cast<int32_t>(canvas_size.x) * 4 +
                    x * 4;
                const auto color = ImColor(image_buffer[offset],
                                           image_buffer[offset + 1],
                                           image_buffer[offset + 2],
                                           image_buffer[offset + 3]);

                if (color != ImColor(0, 0, 0, 0))
                {
                    const auto point =
                        ImVec2(x - canvas_pos.x, y - canvas_pos.y);
                    points.push_back(point);
                }
            }
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        ClearCanvas();
    }

    ImGui::Text("Canvas:");
    ImGui::InvisibleButton("canvas", canvas_size);
    const auto is_mouse_hovering = ImGui::IsItemHovered();

    if (is_mouse_hovering && ImGui::IsMouseDown(0))
    {
        const auto point =
            ImVec2(mouse_pos.x - canvas_pos.x, mouse_pos.y - canvas_pos.y);
        points.push_back(point);
    }

    auto *draw_list = ImGui::GetWindowDrawList();
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

void DrawingTool::ClearCanvas()
{
    std::fill(image_buffer.begin(), image_buffer.end(), 0);
    points.clear();
}

void DrawingTool::SaveToImageFile(std::string_view filename)
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

    WriteImageFile(filename);
}

void DrawingTool::LoadFromImageFile(std::string_view filename)
{
    int width, height, channels;

    // Load the image data from file
    auto *image_data =
        stbi_load(filename.data(), &width, &height, &channels, 4);

    if (!image_data)
    {
        std::cerr << "Error: Could not load the image file!\n";
        return;
    }

    // Update the canvas size
    canvas_size = ImVec2(static_cast<float>(width), static_cast<float>(height));

    // Allocate memory for image_buffer
    image_buffer.assign(image_data, image_data + (width * height * 4));

    // Free the loaded image data
    stbi_image_free(image_data);
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

void DrawingTool::WriteImageFile(std::string_view filename)
{
    stbi_write_bmp(filename.data(),
                   static_cast<int>(canvas_size.x),
                   static_cast<int>(canvas_size.y),
                   4,
                   image_buffer.data());
}

void render(DrawingTool &drawingTool)
{
    drawingTool.Draw("Drawing Editor");
}
