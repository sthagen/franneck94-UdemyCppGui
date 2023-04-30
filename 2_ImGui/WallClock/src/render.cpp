#include <chrono>
#include <string_view>
#include <tuple>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto main_window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto main_window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto main_window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(main_window_size);
    ImGui::SetNextWindowPos(main_window_pos);

    ImGui::Begin(label.data(), nullptr, main_window_flags);

    const auto cursor_pos = ImGui::GetCursorScreenPos();
    center = ImVec2(cursor_pos.x + circleRadius, cursor_pos.y + circleRadius);
    ImGui::GetWindowDrawList()->AddCircle(center,
                                          circleRadius,
                                          ImGui::GetColorU32(ImGuiCol_Text),
                                          100,
                                          2.0F);

    GetTime();
    const auto [hour_theta, minute_theta, second_theta] = GetTheta();

    DrawClockHand(circleRadius * 0.95F,
                  hour_theta,
                  ImColor(1.0f, 0.0F, 0.0f, 1.0f));
    DrawClockHand(circleRadius * 0.85F,
                  minute_theta,
                  ImColor(0.0f, 1.0F, 0.0f, 1.0f));
    DrawClockHand(circleRadius * 0.75F,
                  second_theta,
                  ImColor(0.0f, 0.0F, 1.0f, 1.0f));

    DrawHours();
    DrawMinutes();

    ImGui::GetWindowDrawList()->AddCircle(center,
                                          5.0F,
                                          ImGui::GetColorU32(ImGuiCol_Text),
                                          10,
                                          2.0F);

    ImGui::End();
}

void WindowClass::DrawClockHand(const float radius,
                                const float theta,
                                const ImColor color)
{
    const auto c = std::cos(theta); // x
    const auto s = std::sin(theta); // y
    const auto end_point = ImVec2(center.x - radius * c, center.y - radius * s);

    ImGui::GetWindowDrawList()->AddLine(center, end_point, color, 3.0F);
}

void WindowClass::DrawHours()
{
    for (std::uint8_t hr = 0; hr < 12; ++hr)
    {
        const auto theta = (hr * ((2.0F * PI) / 12.0F)) + offset;
        const auto c = std::cos(theta); // x
        const auto s = std::sin(theta); // y

        const auto start_point = ImVec2(center.x + (circleRadius * 0.9F) * c,
                                        center.y - (circleRadius * 0.9F) * s);
        const auto end_point =
            ImVec2(center.x + circleRadius * c, center.y - circleRadius * s);

        ImGui::GetWindowDrawList()->AddLine(start_point,
                                            end_point,
                                            ImGui::GetColorU32(ImGuiCol_Text),
                                            2.0F);
    }
}

void WindowClass::DrawMinutes()
{
    for (std::uint8_t min = 0; min < 60; ++min)
    {
        const auto theta = (min * ((2.0F * PI) / 60.0F)) + offset;
        const auto c = std::cos(theta); // x
        const auto s = std::sin(theta); // y

        const auto start_point = ImVec2(center.x + (circleRadius * 0.95F) * c,
                                        center.y - (circleRadius * 0.95F) * s);
        const auto end_point =
            ImVec2(center.x + circleRadius * c, center.y - circleRadius * s);

        ImGui::GetWindowDrawList()->AddLine(start_point,
                                            end_point,
                                            ImGui::GetColorU32(ImGuiCol_Text),
                                            1.0F);
    }
}

void WindowClass::GetTime()
{
    const auto timestamp_now = std::chrono::system_clock::now();
    const auto time_now = std::chrono::system_clock::to_time_t(timestamp_now);
    const auto time_struct = std::localtime(&time_now);

    secs = time_struct->tm_sec;
    mins = time_struct->tm_min;
    hrs = time_struct->tm_hour;
}

std::tuple<float, float, float> WindowClass::GetTheta()
{
    const auto seconds_frac = static_cast<float>(secs);
    const auto minutes_frac =
        static_cast<float>(mins) + (static_cast<float>(secs) / 60.0F);
    const auto hours_frac =
        static_cast<float>(hrs) + (static_cast<float>(mins) / 60.0F);

    const auto hour_theta = (hours_frac * ((2.0F * PI) / 12.0F)) + offset;
    const auto minute_theta = (minutes_frac * ((2.0F * PI) / 60.0F)) + offset;
    const auto second_theta = (seconds_frac * ((2.0F * PI) / 60.0F)) + offset;

    return std::make_tuple(hour_theta, minute_theta, second_theta);
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
