#pragma once

#include <cstdint>
#include <string_view>

class WindowClass
{
public:
    constexpr static auto PI = 3.14159f;

public:
    void Draw(std::string_view label);

    void DrawClockHand(const float radius,
                       const float angle,
                       const float thickness,
                       const ImColor color);
};

void render(WindowClass &window_obj);
