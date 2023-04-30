#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "miniaudio.h"
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <implot.h>

#include "MediaPlayer.hpp"

void MediaPlayer::InitEngine()
{
    ma_engine_init(nullptr, &engine);
    engineInitialized = true;
}

void MediaPlayer::Draw(std::string_view label, bool *open)
{
    if (!engineInitialized)
        InitEngine();

    if (isPlaying && !ma_sound_is_playing(&sound))
    {
        isPlaying = false;
    }

    ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);

    ImGui::Begin(label.data(), open, mainWindowFlags);

    ImGui::Text("Audio File Path:");
    ImGui::SameLine();
    ImGui::InputText("##AudioFilePath", &audioFilePath);

    if (ImGui::Button(isPlaying ? "Stop" : "Play"))
    {
        if (isPlaying)
        {
            ma_sound_stop(&sound);
            isPlaying = false;
        }
        else
        {
            if (soundInitialized)
            {
                ma_sound_uninit(&sound);
                soundInitialized = false;
            }

            ma_sound_init_from_file(&engine,
                                    audioFilePath.c_str(),
                                    NULL,
                                    NULL,
                                    NULL,
                                    &sound);
            ma_sound_start(&sound);
            isPlaying = true;
            soundInitialized = true;
        }
    }

    ImGui::End();
}
