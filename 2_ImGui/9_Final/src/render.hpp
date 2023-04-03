#pragma once

#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "miniaudio.h"

class WindowClass
{
public:
    struct Icon
    {
        explicit Icon(const std::string &name)
            : label(name), popup_open(false){};

        void Draw(std::string_view label);

        std::string label;
        int clicked_count = 0;
        bool popup_open;
        ImVec2 position;
    };

public:
    WindowClass()
    {
        for (int i = 0; i < 10; ++i)
        {
            icons.push_back(Icon("Icon " + std::to_string(i + 1)));
        }
    };

    void Draw(std::string_view label);

private:
    std::vector<Icon> icons;
};

void render(WindowClass &window_class);
