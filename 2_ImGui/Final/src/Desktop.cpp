#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "AdvCalc.hpp"
#include "Calender.hpp"
#include "Desktop.hpp"
#include "Diff.hpp"
#include "FileExplorer.hpp"
#include "MediaPlayer.hpp"
#include "Paint.hpp"
#include "TextEditor.hpp"

void Desktop::Draw(std::string_view label, bool *)
{
    constexpr static auto flags = ImGuiWindowFlags_NoDecoration |
                                  ImGuiWindowFlags_NoScrollWithMouse |
                                  ImGuiWindowFlags_NoInputs;
    static auto open_taskbar = false;

    ImGui::SetNextWindowPos(rootPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(fullscreenSize, ImGuiCond_Always);

    ImGui::Begin(label.data(), nullptr, flags);

    for (auto &icon : icons)
        icon.Draw();

    ImGui::SetNextWindowPos(ImVec2(0, 680), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280, 40), ImGuiCond_Always);

    ImGui::Begin("Taskbar",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::SetCursorPosX(0.0F);
    if (ImGui::Button("All Icons", ImVec2(100.0F, 30.0F)))
    {
        ImGui::OpenPopup("My Programs");
        open_taskbar = true;
    }

    ImGui::SameLine();

    static auto theme_open = false;
    if (ImGui::Button("Theme", ImVec2(100.0F, 30.0F)) || theme_open)
    {
        theme_open = true;
        DrawColorsSettings(&theme_open);
    }

    ImGui::SameLine();

    ImGui::SetCursorPosX(fullscreenSize.x - 100.0F);

    static auto clock_open = false;
    clock.GetTime();
    const auto time = fmt::format("{}:{}", clock.hrs, clock.mins);
    if (ImGui::Button(time.data(), ImVec2(100.0F, 30.0F)) || clock_open)
    {
        clock.Draw("clockWindow");
        clock_open = true;
    }
    if (ImGui::IsMouseClicked(0))
    {
        clock_open = false;
    }

    if (open_taskbar)
        ShowIconList(&open_taskbar);

    ImGui::End();

    ImGui::End();
}

void Desktop::ShowIconList(bool *open)
{
    const auto icon_count = static_cast<float>(icons.size());
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * icon_count + 40.0F;

    ImGui::SetNextWindowPos(ImVec2(0, 680 - popup_height), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, popup_height), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs", open))
    {
        for (auto &icon : icons)
        {
            if (ImGui::Selectable(icon.label.data()))
            {
                icon.popup_open = true;
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}

void Desktop::Icon::Draw()
{
    constexpr static auto flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    const auto label_ = fmt::format("Icon Popup Window##{}", label);

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::Begin(fmt::format("###{}", label).data(), nullptr, flags);

    if (ImGui::Button(label.data(), ImVec2(100, 50)) || popup_open)
    {
        popup_open = true;
        base->Draw(label, &popup_open);
    }

    ImGui::End();
}

void render(Desktop &window_class)
{
    window_class.Draw("Desktop");
}
