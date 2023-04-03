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

void WindowClass::Draw(std::string_view title)
{
    static bool open_save_popup = false;
    static bool open_load_popup = false;

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

    ImGui::Text("Draw Size");
    ImGui::SameLine();
    ImGui::SliderFloat("##draw_size", &point_draw_size, 1.0f, 10.0f, "%.1f");

    if (open_save_popup)
    {
        ImGui::OpenPopup("Save Image");
    }
    if (ImGui::BeginPopupModal("Save Image",
                               nullptr,
                               ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("File extension must be .bin");
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
        points.push_back(
            std::make_tuple(point, current_draw_color, point_draw_size));
    }

    auto *draw_list = ImGui::GetWindowDrawList();
    for (const auto &[point, color, size] : points)
    {
        draw_list->AddCircleFilled(
            ImVec2(canvas_pos.x + border_thickness + point.x,
                   canvas_pos.y + border_thickness + point.y),
            size,
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
    points.clear();
}

void WindowClass::SaveToImageFile(std::string_view filename)
{
    std::ofstream file(filename.data(), std::ios::binary);

    if (!file)
    {
        std::cerr << "Error: Could not open the file for writing!\n";
        return;
    }

    auto point_count = points.size();
    file.write(reinterpret_cast<char *>(&point_count), sizeof(point_count));

    for (const auto &[point, color, size] : points)
    {
        file.write(reinterpret_cast<const char *>(&point), sizeof(point));
        file.write(reinterpret_cast<const char *>(&color),
                   sizeof(color));
        file.write(reinterpret_cast<const char *>(&size), sizeof(size));
    }
    file.close();
}

void WindowClass::LoadFromImageFile(std::string_view filename)
{
    auto file = std::ifstream(filename.data(), std::ios::binary);

    if (!file)
    {
        std::cerr << "Error: Could not open the file for reading!\n";
        return;
    }

    auto point_count = std::size_t{};
    file.read(reinterpret_cast<char *>(&point_count), sizeof(point_count));

    ClearCanvas();
    points.reserve(point_count);

    for (std::uint32_t i = 0; i < point_count; ++i)
    {
        auto point = ImVec2{};
        auto color = ImColor{};
        auto size = float{};

        file.read(reinterpret_cast<char *>(&point), sizeof(point));
        file.read(reinterpret_cast<char *>(&color), sizeof(color));
        file.read(reinterpret_cast<char *>(&size), sizeof(size));

        points.push_back(std::make_tuple(point, color, size));
    }
    file.close();
}

void WindowClass::DrawColorButtons()
{
    const auto selected_red = current_draw_color == ImColor(255, 0, 0);
    const auto selected_green = current_draw_color == ImColor(0, 255, 0);
    const auto selected_blue = current_draw_color == ImColor(0, 0, 255);
    const auto selected_white = current_draw_color == ImColor(255, 255, 255);
    const auto none_preset_color =
        !selected_red && !selected_green && !selected_blue && !selected_white;

    constexpr auto orange = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);

    if (selected_red)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Red"))
        current_draw_color = ImColor(255, 0, 0);
    if (selected_red)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    if (selected_green)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Green"))
        current_draw_color = ImColor(0, 255, 0);
    if (selected_green)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    if (selected_blue)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Blue"))
        current_draw_color = ImColor(0, 0, 255);
    if (selected_blue)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    if (selected_white)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("White"))
        current_draw_color = ImColor(255, 255, 255);
    if (selected_white)
        ImGui::PopStyleColor();

    ImGui::SameLine();

    bool open_color_picker_popup = false;
    if (none_preset_color)
        ImGui::PushStyleColor(ImGuiCol_Button, orange);
    if (ImGui::Button("Choose"))
        open_color_picker_popup = true;
    if (none_preset_color)
        ImGui::PopStyleColor();

    if (open_color_picker_popup)
        ImGui::OpenPopup("Color Picker");

    if (ImGui::BeginPopup("Color Picker"))
    {
        ImGui::ColorPicker4("##picker",
                            reinterpret_cast<float *>(&current_draw_color),
                            ImGuiColorEditFlags_NoInputs);
        ImGui::EndPopup();
    }
}

void render(WindowClass &window_class)
{
    window_class.Draw("Drawing Editor");
}
