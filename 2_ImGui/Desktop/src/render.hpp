#pragma once

#include <vector>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

class WindowClass
{
public:
    constexpr static auto numIcons = std::uint32_t{10U};

    struct Icon
    {
        Icon(std::string_view label_) : label(label_), position(ImVec2{}){};
        void Draw();

        std::string label;
        ImVec2 position;
        bool popupOpen = false;
        std::uint32_t clickCount = 0;
    };

public:
    WindowClass()
    {
        for (std::uint32_t i = 0; i < numIcons; ++i)
        {
            icons.push_back(Icon{fmt::format("Icon{}", i)});
        }
    };

    void Draw(std::string_view label);
    void ShowIconList();

private:
    std::vector<Icon> icons;
};

void render(WindowClass &window_class);