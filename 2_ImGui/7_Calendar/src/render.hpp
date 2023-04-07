#pragma once

#include <string_view>

#include <imgui.h>

class WindowClass
{
public:
    void Draw(std::string_view title);
};

void render(WindowClass &window_class);
