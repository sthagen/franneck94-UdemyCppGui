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

    ImGui::SetNextWindowPos(mainPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainSize, ImGuiCond_Always);

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

    if (ImGui::Button("All Icons", ImVec2(100, 20)))
    {
        ImGui::OpenPopup("My Programs");
        open_taskbar = true;
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
