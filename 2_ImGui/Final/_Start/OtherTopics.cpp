#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#include "OtherTopics.hpp"
#include "lodepng.h"

GLuint loadTexture(const char *filename)
{
    std::vector<unsigned char> data;
    unsigned width, height;
    unsigned error = lodepng::decode(data, width, height, filename);

    if (error)
    {
        std::cerr << "Error loading image: " << filename << " (error " << error
                  << ": " << lodepng_error_text(error) << ")" << std::endl;
        return 0;
    }

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
    // glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

void OtherTopics::Draw(std::string_view label)
{
    ImGui::Begin(label.data());

    // if (ImGui::TreeNode("Tabbing"))
    // {
    //     static char buf[32] = "hello";
    //     ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
    //     ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
    //     ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
    //     ImGui::TreePop();
    // }

    // static bool flag1 = false;
    // static bool flag2 = false;

    // if (ImGui::BeginMenu("Examples"))
    // {
    //     ImGui::MenuItem("Main menu bar", NULL, &flag1);
    //     ImGui::MenuItem("Documents", NULL, &flag2);
    //     ImGui::EndMenu();
    // }

    // if (ImGui::CollapsingHeader("Help"))
    // {
    //     ImGui::Text("1");
    //     ImGui::Text("2");
    //     ImGui::Text("3");
    // }

    const auto myImageTexture =
        loadTexture("C:/Users/Jan/OneDrive/_Coding/UdemyCppGui/2_ImGui/Final/"
                    "images/image.png");
    const auto imageWidth = 100;
    const auto imageHeight = 100;
    const auto imageSize =
        ImVec2(static_cast<float>(imageWidth), static_cast<float>(imageHeight));
    ImGui::Image(reinterpret_cast<ImTextureID>(myImageTexture), imageSize);

    // if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
    // {
    //     if (ImGui::BeginTabItem("Sizes1"))
    //     {
    //         ImGui::SeparatorText("Main11");
    //         ImGui::SeparatorText("Main21");
    //         ImGui::EndTabItem();
    //     }
    //     if (ImGui::BeginTabItem("Sizes2"))
    //     {
    //         ImGui::SeparatorText("Main12");
    //         ImGui::SeparatorText("Main22");
    //         ImGui::EndTabItem();
    //     }
    //     ImGui::EndTabBar();
    // }

    ImGui::End();
}
