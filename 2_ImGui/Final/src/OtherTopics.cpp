#include <exception>
#include <iostream>
#include <string_view>
#include <tuple>

#include <imgui.h>
#include <implot.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "lodepng.h"
#include <GLFW/glfw3.h>
#include <fmt/format.h>

#include "OtherTopics.hpp"

static std::tuple<GLuint, std::uint32_t, std::uint32_t> loadTexture(
    const char *filename)
{
    std::vector<unsigned char> data;
    std::uint32_t width, height;
    const auto error = lodepng::decode(data, width, height, filename);

    if (error)
        throw "Error loading image";

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

static void LoadAndDisplayImage(std::string_view image_filepath)
{
    const auto &[myImageTexture, imageWidth, imageHeight] =
        loadTexture(image_filepath.data());
    const auto imageSize =
        ImVec2(static_cast<float>(imageWidth), static_cast<float>(imageHeight));
    ImGui::Image(reinterpret_cast<ImTextureID>(myImageTexture), imageSize);
}

void OtherTopics::Draw(std::string_view label, bool *open)
{
    ImGui::SetNextWindowPos(rootPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(rootSize, ImGuiCond_Always);

    ImGui::Begin(label.data(), open, fullscreenFlags);

    if (ImGui::TreeNode("Tabbing"))
    {
        static char buf[32] = "hello";
        ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
        ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
        ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
        ImGui::TreePop();
    }

    if (ImGui::CollapsingHeader("Help"))
    {
        ImGui::Text("1");
        ImGui::Text("2");
        ImGui::Text("3");
    }

    const auto image_filepath =
        fmt::format("{}{}", PROJECT_PATH, "/images/image.png");
    LoadAndDisplayImage(image_filepath);

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    {
        if (ImGui::BeginTabItem("Sizes1"))
        {
            ImGui::SeparatorText("Main11");
            ImGui::SeparatorText("Main21");
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Sizes2"))
        {
            ImGui::SeparatorText("Main12");
            ImGui::SeparatorText("Main22");
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}
