#pragma once

#include <string_view>
#include <vector>

#include <imgui.h>

#include <fmt/format.h>
#include <implot.h>

#include "clock.hpp"

class WindowClass
{
public:
    constexpr static auto windowFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoTitleBar;
    constexpr static auto windowSize = ImVec2(1280.0F, 720.0F);
    constexpr static auto windowPos = ImVec2(0.0F, 0.0F);

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
    WindowClass() : icons({}), clock({})
    {
        icons.reserve(numIcons);
        for (std::uint32_t i = 0; i < numIcons; ++i)
        {
            icons.push_back(Icon{fmt::format("Icon{}", i)});
        }
    };

    void Draw(std::string_view label);
    void DrawDesktop();
    void DrawTaskbar();

    void ShowIconList(bool *open = nullptr);

private:
    std::vector<Icon> icons;

    Clock clock;
};

void render(WindowClass &window_class);
