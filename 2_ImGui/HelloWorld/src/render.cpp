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
    ImTextureID image_texture = io.Fonts->TexID;
    const auto my_tex_w = (float)io.Fonts->TexWidth;
    const auto my_tex_h = (float)io.Fonts->TexHeight;
    const auto image_size = ImVec2(my_tex_w, my_tex_h);
    ImGui::Image(image_texture, image_size);
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
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
