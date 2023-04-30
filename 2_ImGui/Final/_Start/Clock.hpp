#pragma once

#include <cstdint>
#include <string_view>
#include <tuple>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

class Clock
{
public:
    constexpr static auto PI = 3.14159f;
    constexpr static auto circleRadius = 250.0f;
    constexpr static auto offset = (PI / 2.0F);

public:
    Clock(){};

    void Draw(std::string_view label);

    void DrawClockHand(const float radius,
                       const float angle,
                       const ImColor color);
    void DrawHourStrokes();

    void GetTime();
    std::tuple<float, float, float> GetTheta();

public:
    std::int32_t secs;
    std::int32_t mins;
    std::int32_t hrs;

private:
    ImVec2 center;
};
