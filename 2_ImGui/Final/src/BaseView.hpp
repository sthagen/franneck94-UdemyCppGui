#pragma once

#include <string_view>
#include <algorithm>

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
        SetStyleColors();
    };

    virtual void Draw(std::string_view label, bool *open = nullptr) = 0;

protected:
    void DrawColorsSettings()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImGui::DragFloat("Global Alpha",
                         &style.Alpha,
                         0.05f,
                         0.2f,
                         1.0f,
                         "%.2f");
        style.Alpha = std::clamp(style.Alpha, 0.2f, 1.f);
        ImGui::DragFloat("Global Font Scale",
                         &ImGui::GetIO().FontGlobalScale,
                         0.05f,
                         0.3f,
                         2.0f,
                         "%.1f");
        ImGui::Text("Sizes");
        ImGui::SliderFloat2("Window Padding",
                            (float *)&style.WindowPadding,
                            0.0f,
                            20.0f,
                            "%.0f");
        ImGui::SliderFloat("Window Rounding",
                           &style.WindowRounding,
                           0.0f,
                           16.0f,
                           "%.0f");
        ImGui::SliderFloat2("Frame Padding",
                            (float *)&style.FramePadding,
                            0.0f,
                            20.0f,
                            "%.0f");
        ImGui::SliderFloat("Frame Rounding",
                           &style.FrameRounding,
                           0.0f,
                           16.0f,
                           "%.0f");
        ImGui::SliderFloat2("Item Spacing",
                            (float *)&style.ItemSpacing,
                            0.0f,
                            20.0f,
                            "%.0f");
        ImGui::SliderFloat2("Item InnerSpacing",
                            (float *)&style.ItemInnerSpacing,
                            0.0f,
                            20.0f,
                            "%.0f");
        ImGui::SliderFloat("Indent Spacing",
                           &style.IndentSpacing,
                           0.0f,
                           30.0f,
                           "%.0f");
        ImGui::SliderFloat("Scrollbar Size",
                           &style.ScrollbarSize,
                           1.0f,
                           20.0f,
                           "%.0f");
        ImGui::SliderFloat("Scrollbar Rounding",
                           &style.ScrollbarRounding,
                           0.0f,
                           16.0f,
                           "%.0f");
        ImGui::SliderFloat("Grab MinSize",
                           &style.GrabMinSize,
                           1.0f,
                           20.0f,
                           "%.0f");
        ImGui::SliderFloat("Grab Rounding",
                           &style.GrabRounding,
                           0.0f,
                           16.0f,
                           "%.0f");
        ImGui::Text("Alignment");
        ImGui::SliderFloat2("Window Title Align",
                            (float *)&style.WindowTitleAlign,
                            0.0f,
                            1.0f,
                            "%.2f");
        ImGui::SliderFloat2("Button Text Align",
                            (float *)&style.ButtonTextAlign,
                            0.0f,
                            1.0f,
                            "%.2f");
        ImGui::Text("Colors");
        for (int i = 0; i < ImGuiCol_COUNT; ++i)
        {
            const char *name = ImGui::GetStyleColorName(i);
            ImGui::PushID(i);
            ImGui::ColorEdit4(name,
                              reinterpret_cast<float *>(&style.Colors[i]));
            ImGui::PopID();
        }
    };

    static void SetStyleColors()
    {
        ImGuiStyle &style = ImGui::GetStyle();
        ImVec4 *colors = style.Colors;

        style.Alpha = 1.0f;
        style.DisabledAlpha = 0.60f;
        style.WindowPadding = ImVec2(8, 8);
        style.WindowRounding = 0.0f;
        style.WindowBorderSize = 1.0f;
        style.WindowMinSize = ImVec2(32, 32);
        style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
        style.WindowMenuButtonPosition = ImGuiDir_Left;
        style.ChildRounding = 0.0f;
        style.ChildBorderSize = 1.0f;
        style.PopupRounding = 0.0f;
        style.PopupBorderSize = 1.0f;
        style.FramePadding = ImVec2(4, 3);
        style.FrameRounding = 0.0f;
        style.FrameBorderSize = 0.0f;
        style.ItemSpacing = ImVec2(8, 4);
        style.ItemInnerSpacing = ImVec2(4, 4);
        style.CellPadding = ImVec2(4, 2);
        style.TouchExtraPadding = ImVec2(0, 0);
        style.IndentSpacing = 21.0f;
        style.ColumnsMinSpacing = 6.0f;
        style.ScrollbarSize = 14.0f;
        style.ScrollbarRounding = 9.0f;
        style.GrabMinSize = 12.0f;
        style.GrabRounding = 0.0f;
        style.LogSliderDeadzone = 4.0f;
        style.TabRounding = 4.0f;
        style.TabBorderSize = 0.0f;
        style.TabMinWidthForCloseButton = 0.0f;
        style.ColorButtonPosition = ImGuiDir_Right;
        style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
        style.SeparatorTextBorderSize = 3.0f;
        style.SeparatorTextAlign = ImVec2(0.0f, 0.5f);
        style.SeparatorTextPadding = ImVec2(20.0f, 3.f);
        style.DisplayWindowPadding = ImVec2(19, 19);
        style.DisplaySafeAreaPadding = ImVec2(3, 3);
        style.MouseCursorScale = 1.0f;
        style.AntiAliasedLines = true;
        style.AntiAliasedLinesUseTex = true;
        style.AntiAliasedFill = true;
        style.CurveTessellationTol = 1.25f;
        style.CircleTessellationMaxError = 0.30f;

        colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.5f, 0.5f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.4f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.0f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.0f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.0f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.4f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.0f);
        colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.8f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1f, 0.4f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.1f, 0.4f, 0.75f, 1.0f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.2f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = colors[ImGuiCol_Header];
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = colors[ImGuiCol_HeaderActive];
        colors[ImGuiCol_TabUnfocused] = colors[ImGuiCol_Tab];
        colors[ImGuiCol_TabUnfocusedActive] = colors[ImGuiCol_TabActive];
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.0f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.43f, 0.35f, 1.0f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.9f, 0.7f, 0.0f, 1.0f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6f, 0.0f, 1.0f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.2f, 1.0f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.0f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.0f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.7f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.2f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.8f, 0.8f, 0.8f, 0.35f);
    };
};
