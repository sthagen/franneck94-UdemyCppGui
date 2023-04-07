#pragma once

#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

class BaseView
{
public:
    constexpr static auto mainFlags = ImGuiWindowFlags_NoResize |
                                      ImGuiWindowFlags_NoMove |
                                      ImGuiWindowFlags_NoScrollbar |
                                      ImGuiWindowFlags_NoCollapse;

public:
    virtual void Draw(std::string_view label, bool *open = NULL) = 0;
};
