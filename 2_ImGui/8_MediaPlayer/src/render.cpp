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

void MediaPlayer::loadFile(const std::string &filename)
{
    if (soundBuffer.loadFromFile(filename))
    {
        sound.setBuffer(soundBuffer);
    }
}

void MediaPlayer::play()
{
    sound.play();
}

void MediaPlayer::pause()
{
    sound.pause();
}

void MediaPlayer::stop()
{
    sound.stop();
}

void MediaPlayer::Draw(std::string_view label)
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

void render(MediaPlayer &media_player)
{
    media_player.Draw("Media Player");
}
