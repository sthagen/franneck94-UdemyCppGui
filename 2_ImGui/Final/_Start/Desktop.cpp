#include <string_view>

#include <imgui.h>

#include <fmt/format.h>
#include <implot.h>

#include "Desktop.hpp"

void Desktop::Draw(std::string_view label)
{
    constexpr auto window_flags =
        (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
         ImGuiWindowFlags_NoScrollWithMouse);
    static auto open_taskbar = false;

    ImGui::SetNextWindowSize(ImVec2(1280.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F), ImGuiCond_Always);

    ImGui::Begin(label.data(), NULL, window_flags);

    for (auto &icon : icons)
        icon.Draw();

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280.0F, 40.0F), ImGuiCond_Always);

    ImGui::Begin("Taskbar", NULL, window_flags);

    if (ImGui::Button("All Icons", ImVec2(100, 30)))
    {
        ImGui::OpenPopup("My Programs");
        open_taskbar = true;
    }

    if (open_taskbar)
        ShowIconList(&open_taskbar);

    ImGui::SameLine();

    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x);

    static auto clock_open = false;
    clock.GetTime();
    const auto time = fmt::format("{}:{}", clock.hrs, clock.mins);
    if (ImGui::Button(time.data(), ImVec2(100, 30)) || clock_open)
    {
        clock.Draw("clockWindow");
        clock_open = true;
    }
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        clock_open = false;

    ImGui::End();

    ImGui::End();
}

void Desktop::ShowIconList(bool *open)
{
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * numIcons + 40.0F;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F - popup_height),
                            ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200.0F, popup_height), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs", open))
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

void Desktop::Icon::Draw()
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

void render(Desktop &window_class)
{
    window_class.Draw("Desktop");
}
