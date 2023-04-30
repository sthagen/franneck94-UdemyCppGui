#include <iostream>

#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto mainWindowFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto mainWindowSize = ImVec2(1280.0F, 720.0F);
    constexpr static auto mainWindowPos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(ImVec2(1280.0F, 720.0F));
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F));

    ImGui::Begin(label.data(), nullptr, mainWindowFlags);


    ImGui::End();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
