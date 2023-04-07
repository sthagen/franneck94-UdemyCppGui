#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);

    ImGui::Begin(label.data(), NULL, ImGuiWindowFlags_NoDecoration);

    int i = 0;
    for (auto &icon : icons)
    {
        icon.Draw();
        ++i;
    }
    // Draw taskbar
    ImGui::SetNextWindowPos(ImVec2(0, 680), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280, 40), ImGuiCond_Always);

    ImGui::Begin("Taskbar",
                 NULL,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse);

    if (ImGui::Button("All Icons", ImVec2(100, 20)))
    {
        ImGui::OpenPopup("My Programs");
    }

    ShowIconList();

    ImGui::End();

    ImGui::End();
}

void WindowClass::ShowIconList()
{
    const auto icon_count = static_cast<int>(icons.size());
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * icon_count + 40.0F;

    ImGui::SetNextWindowPos(ImVec2(0, 680 - popup_height), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, popup_height), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs"))
    {
        int i = 0;
        for (auto &icon : icons)
        {
            if (ImGui::Selectable(icon.label.data()))
            {
                icon.popup_open = true;
                ImGui::CloseCurrentPopup();
            }
            ++i;
        }

        ImGui::EndPopup();
    }
}

void WindowClass::Icon::Draw()
{
    const auto label_ = fmt::format("Icon Popup Window##{}", label);

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::Begin(label.data(),
                 nullptr,
                 (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_AlwaysAutoResize));
    if (ImGui::Button(label.data(), ImVec2(100, 50)))
    {
        clicked_count++;
    }
    ImGui::End();

    if (clicked_count >= 1 || popup_open)
    {
        ImGui::OpenPopup(label_.data());
        clicked_count = 0;
        popup_open = true;
    }

    if (ImGui::BeginPopupModal(label_.data(), &popup_open))
    {
        // Add your popup window content here
        ImGui::Text("You opened: %s", label.data());

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            popup_open = false;
        }

        ImGui::EndPopup();
    }
}

void render(WindowClass &window_class)
{
    window_class.Draw("Desktop");
}
