#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "Desktop.hpp"

void Desktop::Draw(std::string_view label, bool *)
{
    constexpr static auto flags = ImGuiWindowFlags_NoDecoration |
                                  ImGuiWindowFlags_NoScrollWithMouse |
                                  ImGuiWindowFlags_NoInputs;
    constexpr static auto button_flags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;
    static auto open_taskbar = false;

    ImGui::SetNextWindowSize(rootSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(rootPos, ImGuiCond_Always);

    ImGui::Begin(label.data(), NULL, flags);

    for (auto &icon : icons)
        icon.Draw();

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280.0F, 40.0F), ImGuiCond_Always);

    ImGui::Begin("Taskbar", NULL, button_flags);

    ImGui::SetCursorPosX(0.0F);
    if (ImGui::Button("All Icons", ImVec2(100.0F, 30.0F)))
    {
        ImGui::OpenPopup("My Programs");
        open_taskbar = true;
    }

    if (open_taskbar)
        ShowIconList(&open_taskbar);

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
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        clock_open = false;

    ImGui::End();

    ImGui::End();
}

void Desktop::ShowIconList(bool *open)
{
    const auto icon_count = static_cast<float>(icons.size());
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * icon_count + 40.0F;

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
    constexpr static auto flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::Begin(fmt::format("###{}", label).data(), nullptr, flags);

    if (ImGui::Button(label.data(), ImVec2(100.0F, 50.0F)) || popupOpen)
    {
        popupOpen = true;
        base->Draw(label, &popupOpen);
    }

    ImGui::End();
}

void render(Desktop &window_class)
{
    window_class.Draw("Desktop");
}
