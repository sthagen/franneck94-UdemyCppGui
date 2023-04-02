#include <iostream>

#include "imgui.h"
#include "implot.h"

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::Begin(label.data());

    if (ImGui::Button("ClickMe"))
    {
        ImGui::Text("Clicked");
    }

    ImGui::End();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
