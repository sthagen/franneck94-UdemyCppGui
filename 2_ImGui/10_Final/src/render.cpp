#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

void MovableButton(const char *label, ImVec2 &position, ImVec2 size)
{
}

void WindowClass::Draw(std::string_view label)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);

    ImGui::Begin("Desktop-like Window", NULL, ImGuiWindowFlags_NoDecoration);

    int i = 0;
    for (auto &icon : icons)
    {
        icon.Draw(fmt::format("Icon{}", i).c_str());
        ++i;
    }

    ImGui::End();
}

void WindowClass::Icon::Draw(std::string_view label)
{
    ImGui::PushID(label.data());
    auto &io = ImGui::GetIO();
    auto *draw_list = ImGui::GetWindowDrawList();

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::Begin(label.data(),
                 nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::Button(label.data(), ImVec2(100, 50)))
    {
        clicked_count++;
    }
    ImGui::End();

    if (clicked_count >= 2)
    {
        ImGui::OpenPopup("Popup Window");
        clicked_count = 0;
        popup_open = true;
    }

    if (ImGui::BeginPopupModal("Popup Window", &popup_open))
    {
        // Add your popup window content here
        ImGui::Text("This is a popup window!");

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            popup_open = false;
        }

        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Media Player");
}
