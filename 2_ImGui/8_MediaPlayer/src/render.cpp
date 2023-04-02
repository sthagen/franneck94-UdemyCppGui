#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <imgui-SFML.h>

#include "render.hpp"

void WindowClass::loadFile(const std::string &filename)
{
    if (soundBuffer.loadFromFile(filename))
    {
        sound.setBuffer(soundBuffer);
    }
}

void WindowClass::play()
{
    sound.play();
}

void WindowClass::pause()
{
    sound.pause();
}

void WindowClass::stop()
{
    sound.stop();
}

void WindowClass::Draw(std::string_view label)
{
    ImGui::Begin(label.data());

    if (ImGui::Button("Play"))
    {
        play();
    }
    ImGui::SameLine();
    if (ImGui::Button("Pause"))
    {
        pause();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        stop();
    }

    ImGui::End();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Media Player");
}
