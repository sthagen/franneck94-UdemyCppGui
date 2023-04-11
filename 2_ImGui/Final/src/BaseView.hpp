#pragma once

#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

class BaseView
{
public:
    constexpr static auto mainFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    constexpr static auto mainPos = ImVec2(0.0F, 0.0F);
    constexpr static auto mainSize = ImVec2(1280.0F, 720.0F);

public:
    virtual void Draw(std::string_view label, bool *open = nullptr) = 0;
};
