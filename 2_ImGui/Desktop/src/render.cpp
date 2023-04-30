#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(windowPos);

    ImGui::Begin(label.data(),
                 nullptr,
                 windowFlags | ImGuiWindowFlags_NoInputs);

    DrawDesktop();
    DrawTaskbar();

    ImGui::End();
}

void WindowClass::DrawDesktop()
{
    for (auto &icon : icons)
        icon.Draw();
}

void WindowClass::DrawTaskbar()
{
    static auto open_taskbar = false;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280.0F, 40.0F), ImGuiCond_Always);

    ImGui::Begin("Taskbar", NULL, windowFlags);

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
    if (clock_open && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        clock_open = false;

    ImGui::End();
}

void WindowClass::ShowIconList(bool *open)
{
    const auto selectable_height = ImGui::GetTextLineHeightWithSpacing();
    const auto popup_height = selectable_height * numIcons + 40.0F;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 680.0F - popup_height),
                            ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(100.0F, popup_height), ImGuiCond_Always);

    if (ImGui::BeginPopupModal("My Programs", open, ImGuiWindowFlags_NoResize))
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
    constexpr static auto button_size = ImVec2(100.0F, 50.0F);
    const auto label_popup = fmt::format("Icon Popop Window##{}", label);

    ImGui::SetNextWindowPos(position, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(
        ImVec2(button_size.x + 35.0F, button_size.y + 35.0F),
        ImGuiCond_FirstUseEver);
    ImGui::Begin(label.data(),
                 nullptr,
                 (ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                  ImGuiWindowFlags_NoResize));

    if (ImGui::Button(label.data(), button_size))
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
