#include <exception>
#include <iostream>
#include <string_view>
#include <tuple>

#include "imgui.h"
#include "implot.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "lodepng.h"
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include "Desktop.hpp"

static std::tuple<GLuint, std::uint32_t, std::uint32_t> loadTexture(
    const char *filename)
{
    std::vector<unsigned char> data;
    std::uint32_t width, height;
    const auto error = lodepng::decode(data, width, height, filename);

    if (error)
        throw std::exception("Error loading image");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 width,
                 height,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 &data[0]);

    return std::make_tuple(texture, width, height);
}

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

    ImGui::SetCursorPos(ImVec2(0.0F, 0.0F));
    const auto &[myImageTexture, imageWidth, imageHeight] =
        loadTexture("C:/Users/Z0014496/Documents/_LocalCoding/UdemyCppGui/"
                    "2_ImGui/Final/images/bg.png");
    const auto imageSize =
        ImVec2(static_cast<float>(imageWidth), static_cast<float>(imageHeight));
    ImGui::Image(reinterpret_cast<ImTextureID>(myImageTexture), imageSize);
    ImGui::SetCursorPos(ImVec2(0.0F, 0.0F));

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

    static auto theme_open = false;
    if (ImGui::Button("Theme", ImVec2(100.0F, 30.0F)) || theme_open)
    {
        theme_open = true;
        DrawColorsSettings(&theme_open);
    }

    ImGui::SameLine();

    ImGui::SetCursorPosX(rootSize.x - 100.0F);

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
