#include <iostream>

#include "imgui.h"
#include "implot.h"

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::Begin("w1");
    if (ImGui::TreeNode("Tabbing"))
    {
        ImGui::Text(
            "Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
        static char buf[32] = "hello";
        ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
        ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
        ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
        ImGui::TreePop();
    }
    ImGui::End();

    ImGui::Begin("w2");
    static bool flag1 = false;
    static bool flag2 = false;

    if (ImGui::BeginMenu("Examples"))
    {
        ImGui::MenuItem("Main menu bar", NULL, &flag1);
        ImGui::MenuItem("Documents", NULL, &flag2);
        ImGui::EndMenu();
    }
    ImGui::End();

    ImGui::Begin("w3");
    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("ABOUT THIS DEMO:");
    }
    ImGui::End();

    ImGui::Begin("w4");
    ImGuiIO &io = ImGui::GetIO();
    ImTextureID my_tex_id = io.Fonts->TexID;
    float my_tex_w = (float)io.Fonts->TexWidth;
    float my_tex_h = (float)io.Fonts->TexHeight;
    ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);               // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);               // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(my_tex_id,
                 ImVec2(my_tex_w, my_tex_h),
                 uv_min,
                 uv_max,
                 tint_col,
                 border_col);
    ImGui::End();

    ImGui::Begin("w5");
    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Sizes1"))
        {
            ImGui::SeparatorText("Main1");
            ImGui::SeparatorText("Main2");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sizes2"))
        {
            ImGui::SeparatorText("Main1");
            ImGui::SeparatorText("Main2");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();

    ImGui::Begin("w6");
    static bool act = false;
    if (ImGui::RadioButton("rad", act))
        act = true;
    ImGui::ArrowButton("arr", ImGuiDir_Left);
    ImGui::SmallButton("small");
    ImGui::End();

    ImGui::Begin("w7");
    char n[] = "jan";
    ImGui::Text("From %s", n);
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("DND_DEMO_CELL", n, sizeof(n));
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const auto *payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            auto *payload_n = (const char *)payload->Data;
            ImGui::Text("To %s", payload_n);
        }
        ImGui::EndDragDropTarget();
    }
    ImGui::End();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
