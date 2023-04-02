#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include "render.hpp"

void WindowClass::Draw(std::string_view title)
{
    ImGui::Begin(title.data(),
                 NULL,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    const auto mouse_pos = ImGui::GetMousePos();

    if (ImGui::Button("Save"))
    {
        open_save_popup = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
        open_load_popup = true;
    }
    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        ClearCanvas();
    }

    DrawColorButtons();

    if (open_save_popup)
    {
        ImGui::OpenPopup("Save Image");
    }
    if (ImGui::BeginPopupModal("Save Image",
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("Filename", filename_buffer, sizeof(filename_buffer));
        if (ImGui::Button("Save"))
        {
            SaveToImageFile(filename_buffer);
            open_save_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            open_save_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (open_load_popup)
    {
        ImGui::OpenPopup("Load Image");
    }
    if (ImGui::BeginPopupModal("Load Image",
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::InputText("Filename", filename_buffer, sizeof(filename_buffer));
        if (ImGui::Button("Load"))
        {
            LoadFromImageFile(filename_buffer);
            open_load_popup = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            open_load_popup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    canvas_pos = ImGui::GetCursorScreenPos();
    const auto button_size = ImVec2(canvas_size.x + border_thickness * 2,
                                    canvas_size.y + border_thickness * 2);
    ImGui::InvisibleButton("canvas", button_size);

    const auto is_mouse_hovering = ImGui::IsItemHovered();
    if (is_mouse_hovering && ImGui::IsMouseDown(0))
    {
        const auto point =
            ImVec2(mouse_pos.x - canvas_pos.x - border_thickness,
                   mouse_pos.y - canvas_pos.y - border_thickness);
        points.push_back(std::make_pair(point, current_draw_color));
    }

    auto *draw_list = ImGui::GetWindowDrawList();
    for (const auto &[point, color] : points)
    {
        draw_list->AddCircleFilled(
            ImVec2(canvas_pos.x + border_thickness + point.x,
                   canvas_pos.y + border_thickness + point.y),
            2.0f,
            color,
            16);
    }

    const auto border_min = ImVec2(canvas_pos.x, canvas_pos.y);
    const auto border_max =
        ImVec2(canvas_pos.x + button_size.x - border_thickness,
               canvas_pos.y + button_size.y - border_thickness);
    draw_list->AddRect(border_min,
                       border_max,
                       IM_COL32(255, 255, 255, 255),
                       0.0f,
                       ImDrawCornerFlags_All,
                       border_thickness);

    ImGui::End();
}

void WindowClass::ClearCanvas()
{
    std::fill(image_buffer.begin(), image_buffer.end(), 0);
    points.clear();
}

void WindowClass::SaveToImageFile(std::string_view filename)
{
    for (const auto &[point, color] : points)
    {
        const auto draw_pos =
            ImVec2(canvas_pos.x + point.x, canvas_pos.y + point.y);
        const auto rounded_pos =
            ImVec2(std::floor(draw_pos.x), std::floor(draw_pos.y));
        AddPixel(rounded_pos, color, image_buffer);
    }

    stbi_write_png(filename.data(),
                   num_rows,
                   num_cols,
                   num_channels,
                   image_buffer.data(),
                   num_rows * num_channels);
}

void WindowClass::LoadFromImageFile(std::string_view filename)
{
    int width, height, channels;

    auto *image_data =
        stbi_load(filename.data(), &width, &height, &channels, num_channels);

    if (!image_data)
    {
        std::cerr << "Error: Could not load the image file!\n";
        return;
    }

    // Update the canvas size
    canvas_size = ImVec2(static_cast<float>(width), static_cast<float>(height));

    // Allocate memory for image_buffer
    image_buffer.assign(image_data,
                        image_data + (width * height * num_channels));

    // Free the loaded image data
    stbi_image_free(image_data);

    points.clear();
    for (int32_t y = 0; y < canvas_size.y; ++y)
    {
        for (int32_t x = 0; x < canvas_size.x; ++x)
        {
            const auto offset =
                ((static_cast<int32_t>(canvas_size.y) - y - 1) *
                     static_cast<int32_t>(canvas_size.x) * num_channels +
                 x * num_channels);
            const auto color = ImColor(image_buffer[offset],
                                       image_buffer[offset + 1],
                                       image_buffer[offset + 2]);

            if (color.Value.x != 0 || color.Value.y != 0 || color.Value.z != 0)
            {
                const auto point = ImVec2(x - canvas_pos.x, y - canvas_pos.y);
                points.push_back(std::make_pair(point, color));
            }
        }
    }
}

void WindowClass::AddPixel(const ImVec2 &pos,
                           const ImColor &color,
                           std::vector<uint8_t> &buffer)
{
    const auto x = static_cast<int>(pos.x);
    const auto y = static_cast<int>(pos.y);

    if (x < 0 && x >= canvas_size.x || y < 0 && y >= canvas_size.y)
        return;

    const auto offset =
        static_cast<int>((num_cols - y - 1) * num_rows + x) * num_channels;

    if (offset > num_rows * num_cols * num_channels)
        return;

    buffer[offset + 0] = static_cast<uint8_t>(color.Value.x);
    buffer[offset + 1] = static_cast<uint8_t>(color.Value.y);
    buffer[offset + 2] = static_cast<uint8_t>(color.Value.z);
}

void WindowClass::DrawColorButtons()
{
    const auto selected_red = current_draw_color == ImColor(255, 0, 0);
    const auto selected_green = current_draw_color == ImColor(0, 255, 0);
    const auto selected_blue = current_draw_color == ImColor(0, 0, 255);
    const auto selected_white = current_draw_color == ImColor(255, 255, 255);

    if (selected_red)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    if (ImGui::Button("Red"))
        current_draw_color = ImColor(255, 0, 0);
    if (selected_red)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    if (selected_green)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    if (ImGui::Button("Green"))
        current_draw_color = ImColor(0, 255, 0);
    if (selected_green)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    if (selected_blue)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    if (ImGui::Button("Blue"))
        current_draw_color = ImColor(0, 0, 255);
    if (selected_blue)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    if (selected_white)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    if (ImGui::Button("White"))
        current_draw_color = ImColor(255, 255, 255);
    if (selected_white)
        ImGui::PopStyleColor();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Drawing Editor");
}
