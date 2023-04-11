#pragma once

#include <cstdint>
#include <string_view>
#include <tuple>

#include <imgui.h>

class WindowClass
{
public:
    constexpr static auto PI = 3.14159f;
    constexpr static auto circleRadius = 250.0f;

public:
    void Draw(std::string_view label);

    void DrawClockHand(const float radius,
                       const float angle,
                       const ImColor color);
    void DrawHourStrokes();

    std::tuple<float, float, float> GetTheta();

private:
    ImVec2 center;
};

void render(WindowClass &window_obj);
