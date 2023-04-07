#pragma once

#include <random>
#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

class WindowClass
{
public:
    constexpr static auto num_icons = 10;

    struct Icon
    {
        Icon(std::string_view label_) : label(label_){};
        void Draw();

        int clicked_count = 0;
        bool popup_open = false;
        ImVec2 position;
        std::string label;
    };

public:
    WindowClass()
    {
        for (int i = 0; i < num_icons; ++i)
            icons.push_back(Icon{fmt::format("Icon{}", i)});
    };

    void Draw(std::string_view label);
    void ShowIconList();

private:
    std::vector<Icon> icons;
};

void render(WindowClass &window_class);
