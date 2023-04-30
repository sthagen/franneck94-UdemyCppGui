#pragma once

#include <numbers>
#include <string_view>
#include <tuple>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

class Clock
{
public:
    constexpr static auto PI = std::numbers::pi_v<float>;
    constexpr static auto circleRadius = 250.0F;
    constexpr static auto offset = PI / 2.0F;

public:
    void Draw(std::string_view label);

    void DrawClockHand(const float radius,
                       const float theta,
                       const ImColor color);

    void DrawHours();
    void DrawMinutes();

    void GetTime();
    std::tuple<float, float, float> GetTheta();

public:
    std::int32_t secs = 0;
    std::int32_t mins = 0;
    std::int32_t hrs = 0;

private:
    ImVec2 center = ImVec2{};
};
