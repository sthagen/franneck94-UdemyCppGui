#include <chrono>
#include <cmath>
#include <iostream>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "Clock.hpp"

void Clock::Draw(std::string_view label)
{
    static auto is_open = false;

    ImGui::SetNextWindowSize(ImVec2(1280.0F, 720.0F));
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F));

    ImGui::Begin(label.data(), &is_open);

    const auto cursor_pos = ImGui::GetCursorScreenPos();
    center = ImVec2(cursor_pos.x + circleRadius, cursor_pos.y + circleRadius);
    ImGui::GetWindowDrawList()->AddCircle(center,
                                          circleRadius,
                                          ImGui::GetColorU32(ImGuiCol_Text),
                                          100,
                                          2.0f);

    ImGui::SetCursorScreenPos(center);

    const auto [hour_theta, minute_theta, second_theta] = GetTheta();
    DrawClockHand(circleRadius * 0.95F,
                  hour_theta,
                  ImColor(1.0f, 0.0f, 0.0f, 1.0f));
    DrawClockHand(circleRadius * 0.85F,
                  minute_theta,
                  ImColor(0.0f, 1.0f, 0.0f, 1.0f));
    DrawClockHand(circleRadius * 0.75F,
                  second_theta,
                  ImColor(0.0f, 0.0f, 1.0f, 1.0f));

    DrawHourStrokes();

    ImGui::SetCursorPosY(ImGui::GetCursorScreenPos().y + 50);

    ImGui::End();
}

void Clock::GetTime()
{
    const auto now = std::chrono::system_clock::now();
    const auto time_now = std::chrono::system_clock::to_time_t(now);
    const auto time_parts = std::localtime(&time_now);

    secs = time_parts->tm_sec;
    mins = time_parts->tm_min;
    hrs = time_parts->tm_hour;
}

std::tuple<float, float, float> Clock::GetTheta()
{
    GetTime();

    const auto seconds_frac = secs;
    const auto minutes_frac = mins + (secs / 60.F);
    const auto hours_frac = (hrs % 12) + (mins / 60.0f);

    auto hour_theta = (hours_frac * ((2.0F * PI) / 12.0F)) + offset;
    auto minute_theta = ((minutes_frac * ((2.0F * PI) / 60.0F)) + offset);
    auto second_theta = ((seconds_frac * ((2.0F * PI) / 60.0F)) + offset);

    return std::make_tuple(hour_theta, minute_theta, second_theta);
}

void Clock::DrawClockHand(const float radius,
                          const float theta,
                          const ImColor color)
{
    const auto c = std::cos(theta);
    const auto s = std::sin(theta);
    const auto end_point = ImVec2(center.x - radius * c, center.y - radius * s);

    ImGui::GetWindowDrawList()->AddLine(center, end_point, color, 3.0F);
}

void Clock::DrawHourStrokes()
{
    for (std::uint8_t hr = 0; hr < 12; ++hr)
    {
        const auto theta = (hr * ((2.0F * PI) / 12.0F)) + (PI / 2.0F);
        const auto start_point =
            ImVec2(center.x + (circleRadius * 0.9f) * std::cos(theta),
                   center.y - (circleRadius * 0.9f) * std::sin(theta));
        const auto end_point =
            ImVec2(center.x + circleRadius * std::cos(theta),
                   center.y - circleRadius * std::sin(theta));
        ImGui::GetWindowDrawList()->AddLine(start_point,
                                            end_point,
                                            ImGui::GetColorU32(ImGuiCol_Text),
                                            2.0f);
    }
}
