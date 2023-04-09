#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr auto window_flags =
        (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
         ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::SetNextWindowSize(ImVec2(1280.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F), ImGuiCond_Always);

    ImGui::Begin(label.data(), NULL, window_flags);

    for (auto &icon : icons)
        icon.Draw();

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280.0F, 40.0F), ImGuiCond_Always);

    ImGui::Begin("Taskbar", NULL, window_flags);

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
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * numIcons + 40.0F;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F - popup_height),
                             ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200.0F, popup_height), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs"))
    {
        for (auto &icon : icons)
        {
            if (ImGui::Selectable(icon.label.data()))
            {
                icon.popupOpen = true;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}

void WindowClass::Icon::Draw()
{
    const auto label_popup = fmt::format("Icon Popop Window##{}", label);

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);

    ImGui::Begin(label.data());

    if (ImGui::Button(label.data(), ImVec2(100.0F, 50.0F)))
    {
        ++clickCount;
    }

    ImGui::End();

    if (clickCount >= 1 || popupOpen)
    {
        ImGui::OpenPopup(label_popup.data());
        clickCount = 0;
        popupOpen = true;
    }

    if (ImGui::BeginPopupModal(label_popup.data(), &popupOpen))
    {
        ImGui::Text("Hi");

        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            popupOpen = false;
        }

        ImGui::EndPopup();
    }
}

void render(WindowClass &window_class)
{
    window_class.Draw("Desktop");
}
