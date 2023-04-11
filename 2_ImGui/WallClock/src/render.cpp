#include <chrono>
#include <cmath>
#include <iostream>

#include "imgui.h"
#include "implot.h"
#include <fmt/format.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    const auto now = std::chrono::system_clock::now();
    const auto time_now = std::chrono::system_clock::to_time_t(now);
    const auto time_parts = std::localtime(&time_now);

    const auto hour_angle =
        (time_parts->tm_hour % 12) * 2.0f * PI / 12.0f - PI / 2.0f;
    const auto minute_angle =
        time_parts->tm_min * 2.0f * PI / 60.0f - PI / 2.0f;
    const auto second_angle =
        time_parts->tm_sec * 2.0f * PI / 60.0f - PI / 2.0f;

    ImGui::Begin(label.data());

    const auto cursor_pos = ImGui::GetCursorScreenPos();
    const auto center = ImVec2(cursor_pos.x + 50, cursor_pos.y + 50);
    ImGui::GetWindowDrawList()->AddCircle(center,
                                          50.0f,
                                          ImGui::GetColorU32(ImGuiCol_Text),
                                          100,
                                          2.0f);

    ImGui::SetCursorScreenPos(center);
    DrawClockHand(30.0f, hour_angle, 3.0f, ImColor(1.0f, 0.0f, 0.0f, 1.0f));
    DrawClockHand(40.0f, minute_angle, 2.0f, ImColor(0.0f, 1.0f, 0.0f, 1.0f));
    DrawClockHand(45.0f, second_angle, 1.0f, ImColor(0.0f, 0.0f, 1.0f, 1.0f));

    ImGui::End();
}

void WindowClass::DrawClockHand(const float radius,
                                const float angle,
                                const float thickness,
                                const ImColor color)
{
    const auto center = ImGui::GetCursorScreenPos();
    const auto end_point = ImVec2(center.x + radius * std::cos(angle),
                                  center.y - radius * std::sin(angle));

    ImGui::GetWindowDrawList()->AddLine(center, end_point, color, thickness);
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
