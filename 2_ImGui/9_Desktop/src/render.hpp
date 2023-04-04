#pragma once

#include <random>
#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

class WindowClass
{
public:
    constexpr static auto num_icons = 10;

    struct Icon
    {
        void Draw(std::string_view label);

        int clicked_count = 0;
        bool popup_open = false;
        ImVec2 position;
    };

public:
    WindowClass()
    {
        for (int i = 0; i < num_icons; ++i)
            icons.push_back(Icon{});
    };

    void Draw(std::string_view label);

private:
    std::vector<Icon> icons;
};

void render(WindowClass &window_class);
