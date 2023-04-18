#pragma once

#include <cstdint>
#include <cmath>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#ifdef RGB
#undef RGB
#endif

namespace Colors
{

static ImU32 ARGB(const std::int32_t a,
                  const std::int32_t r,
                  const std::int32_t g,
                  const std::int32_t b)
{
    return (static_cast<ImU32>(a) << IM_COL32_A_SHIFT) |
           (static_cast<ImU32>(r) << IM_COL32_R_SHIFT) |
           (static_cast<ImU32>(g) << IM_COL32_G_SHIFT) |
           (static_cast<ImU32>(b) << IM_COL32_B_SHIFT);
}

static ImU32 RGB(const std::int32_t r,
                 const std::int32_t g,
                 const std::int32_t b)
{
    return (0xFFu << IM_COL32_A_SHIFT) |
           (static_cast<ImU32>(r) << IM_COL32_R_SHIFT) |
           (static_cast<ImU32>(g) << IM_COL32_G_SHIFT) |
           (static_cast<ImU32>(b) << IM_COL32_B_SHIFT);
}

static ImU32 Black()
{
    return RGB(0, 0, 0);
}

static ImU32 White()
{
    return RGB(0xFF, 0xFF, 0xFF);
}

static ImU32 Empty()
{
    return ARGB(0, 0, 0, 0);
}

static ImU32 Red()
{
    return RGB(0xFF, 0x0, 0x0);
}

static ImU32 Green()
{
    return RGB(0x0, 0xFF, 0x0);
}

static ImU32 Yellow()
{
    return RGB(0xFF, 0xFF, 0x0);
}

static ImU32 Blue()
{
    return RGB(0x0, 0x0, 0xFF);
}

static void ConvertU32ToInt4(const ImU32 color, std::int32_t *i)
{
    i[0] = static_cast<std::int32_t>((color >> IM_COL32_A_SHIFT) & 0xFF);
    i[1] = static_cast<std::int32_t>((color >> IM_COL32_R_SHIFT) & 0xFF);
    i[2] = static_cast<std::int32_t>((color >> IM_COL32_G_SHIFT) & 0xFF);
    i[3] = static_cast<std::int32_t>((color >> IM_COL32_B_SHIFT) & 0xFF);
}

static ImU32 ConvertInt4ToU32(const std::int32_t *i)
{
    return static_cast<ImU32>((i[0] & 0xFF) << IM_COL32_A_SHIFT) |
           static_cast<ImU32>((i[1] & 0xFF) << IM_COL32_R_SHIFT) |
           static_cast<ImU32>((i[2] & 0xFF) << IM_COL32_G_SHIFT) |
           static_cast<ImU32>((i[3] & 0xFF) << IM_COL32_B_SHIFT);
}

static bool DrawSettingHueWheel(const char *text,
                                ImU32 *color,
                                ImGuiColorEditFlags flags = 0)
{
    ImVec4 col = ImGui::ColorConvertU32ToFloat4(*color);
    if (ImGui::ColorEdit4(text,
                          &col.x,
                          flags | ImGuiColorEditFlags_AlphaPreview))
    {
        *color = ImGui::ColorConvertFloat4ToU32(col);
        return true;
    }
    return false;
}

static bool DrawSetting(const char *text, ImU32 *color, bool alpha = true)
{
    std::int32_t i[4];
    ConvertU32ToInt4(*color, i);

    const ImGuiStyle &style = ImGui::GetStyle();

    const std::int32_t n_components = alpha ? 4 : 4;

    bool value_changed = false;

    const float w_full = ImGui::CalcItemWidth();
    const float square_sz = ImGui::GetFontSize() + style.FramePadding.y * 2.0f;

    const float w_items_all = w_full - (square_sz + style.ItemInnerSpacing.x);
    const float w_item_one = std::round(
        (w_items_all - style.ItemInnerSpacing.x * (n_components - 1)) /
        (float)n_components);
    const float w_item_last =
        std::round(w_items_all - (w_item_one + style.ItemInnerSpacing.x) *
                                     (n_components - 1));

    const char *ids[4] = {"##X", "##Y", "##Z", "##W"};
    const char *fmt[4] = {"A:%3.0f", "R:%3.0f", "G:%3.0f", "B:%3.0f"};

    ImGui::BeginGroup();
    ImGui::PushID(text);

    ImGui::PushItemWidth(w_item_one);
    if (alpha)
    {
        value_changed |= ImGui::DragInt("##A", &i[0], 1.0f, 0, 255, "A:%d");
        ImGui::SameLine(0, style.ItemInnerSpacing.x);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Alpha channel (0 - 255)\n0 is transparent, 255 "
                              "is solid color");
    }
    else
    {
        if (!alpha)
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + w_item_one +
                                 style.ItemInnerSpacing.x);
    }

    value_changed |= ImGui::DragInt("##R", &i[1], 1.0f, 0, 255, "R:%d");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Red channel (0 - 255)");

    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    value_changed |= ImGui::DragInt("##G", &i[2], 1.0f, 0, 255, "G:%d");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Green channel (0 - 255)");

    ImGui::PopItemWidth();

    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    ImGui::PushItemWidth(w_item_last);
    value_changed |= ImGui::DragInt("##B", &i[3], 1.0f, 0, 255, "B:%d");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Blue channel (0 - 255)");
    ImGui::PopItemWidth();

    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    ImGui::ColorButton("", ImColor(i[1], i[2], i[3]));

    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("ImU32:\n0x%02X%02X%02X%02X", i[0], i[1], i[2], i[3]);

    ImGui::SameLine(0, style.ItemInnerSpacing.x);
    ImGui::Text(text);

    ImGui::PopID();
    ImGui::EndGroup();

    if (value_changed)
        *color = ConvertInt4ToU32(i);

    return value_changed;
}

static void Clamp(std::int32_t c[4])
{
    for (std::int32_t i = 0; i < 4; ++i)
    {
        if (c[i] < 0)
            c[i] = 0;
        if (c[i] > 0xFF)
            c[i] = 0xFF;
    }
}
static ImU32 Add(const ImU32 &c1, const ImU32 &c2)
{
    std::int32_t i1[4];
    std::int32_t i2[4];
    std::int32_t i3[4]{};
    ConvertU32ToInt4(c1, i1);
    ConvertU32ToInt4(c2, i2);
    for (std::int32_t i = 0; i < 4; ++i)
    {
        i3[i] = i1[i] + i2[i];
    }
    Clamp(i3);
    return ConvertInt4ToU32(i3);
}
static ImU32 Sub(const ImU32 &c1, const ImU32 &c2)
{
    std::int32_t i1[4];
    std::int32_t i2[4];
    std::int32_t i3[4]{};
    ConvertU32ToInt4(c1, i1);
    ConvertU32ToInt4(c2, i2);
    for (std::int32_t i = 0; i < 4; ++i)
    {
        i3[i] = i1[i] - i2[i];
    }
    Clamp(i3);
    return ConvertInt4ToU32(i3);
}

static ImU32 Slerp(const ImU32 &c1, const ImU32 &c2, float t)
{
    std::int32_t i1[4];
    std::int32_t i2[4];
    std::int32_t i3[4]{};
    ConvertU32ToInt4(c1, i1);
    ConvertU32ToInt4(c2, i2);
    for (std::int32_t i = 0; i < 4; ++i)
    {
        i3[i] = (std::int32_t)(i1[i] + (i2[i] - i1[i]) * t);
    }
    return ConvertInt4ToU32(i3);
}
} // namespace Colors
