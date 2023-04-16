#pragma once

#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

class BaseView
{
public:
    constexpr static auto mainFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    constexpr static auto mainPos = ImVec2(0.0F, 0.0F);
    constexpr static auto mainSize = ImVec2(1280.0F, 720.0F);

public:
    BaseView()
    {
        SetStyleTheme();
    };

    virtual void Draw(std::string_view label, bool *open = nullptr) = 0;

private:
    static void SetStyleTheme()
    {
        auto &style = ImGui::GetStyle();
        style.WindowRounding = 6.0f;
        style.FrameRounding = 2.0f;
        style.ScrollbarSize = 16.0f;
        style.ScrollbarRounding = 4.0f;
        style.GrabMinSize = 17.0f;
        style.GrabRounding = 2.0f;
        // style.Colors[ImGuiCol_WindowBg] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_TitleBg] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_TitleBgActive] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_MenuBarBg] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_ScrollbarBg] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_ScrollbarGrab] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_SliderGrabActive] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_Button] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_ButtonHovered] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_ButtonActive] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_Header] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_HeaderHovered] = ImColor(0x00000000);
        // style.Colors[ImGuiCol_HeaderActive] = ImColor(0x00000000);
    };
};
