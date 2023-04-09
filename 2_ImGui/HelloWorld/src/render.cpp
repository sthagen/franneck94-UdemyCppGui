#include <iostream>

#include "imgui.h"
#include "implot.h"

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::SetNextWindowSize(ImVec2(800.0F, 500.0F));
    ImGui::SetNextWindowPos(ImVec2(400.0f, 200.0F));
    ImGui::Begin("MainWindow", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetCursorPos(ImVec2(400.0F, 200.0F));
    if (ImGui::Button("Click me", ImVec2(100.0, 100.0F)))
    {
        ImGui::OpenPopup("Clicked!");
    }

    bool open = true;
    if (ImGui::BeginPopupModal("Clicked!", &open))
    {
        ImGui::Text("You clicked me!");

        ImGui::EndPopup();
    }

    ImGui::End();
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
