#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);

    ImGui::Begin("Desktop-like Window");

    for (auto &icon : icons)
    {
        icon.Draw(fmt::format("###{}", icon.label).c_str());
    }

    ImGui::End();
}

void WindowClass::Icon::Draw(std::string_view label)
{
    ImGui::PushID(label.data());

    // Check for double click
    if (ImGui::Button(label.data()))
    {
        clicked_count++;
    }

    if (clicked_count == 2)
    {
        popup_open = true;
        clicked_count = 0;
    }

    // Check for drag and drop
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        ImGui::SetDragDropPayload("IconPayload",
                                  &label,
                                  sizeof(std::string_view));
        ImGui::Text("Dragging %s", label.data());
        ImGui::EndDragDropSource();
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload *payload =
                ImGui::AcceptDragDropPayload("IconPayload"))
        {
            IM_ASSERT(payload->DataSize == sizeof(std::string_view));
            position = ImGui::GetMousePos();
        }
        ImGui::EndDragDropTarget();
    }

    // Update icon position
    ImGui::SetCursorScreenPos(position);

    // Popup window
    if (popup_open)
    {
        ImGui::OpenPopup(label.data());
    }

    if (ImGui::BeginPopupModal(label.data(), &popup_open))
    {
        ImGui::Text("This is a popup window for %s", label.data());
        if (ImGui::Button("Close"))
        {
            popup_open = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::PopID();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Media Player");
}
