#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    ImGui::End();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Calender Tool");
}
