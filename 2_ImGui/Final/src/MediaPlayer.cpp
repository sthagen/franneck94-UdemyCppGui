#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "miniaudio.h"

#include "MediaPlayer.hpp"

void MediaPlayer::Load()
{
    ma_engine_init(NULL, &engine);
    engineInitialized = true;
}

void MediaPlayer::Draw(std::string_view label, bool *open)
{
    if (!engineInitialized)
        Load();

    if (isPlaying && !ma_sound_is_playing(&sound))
    {
        isPlaying = false;
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_Always);

    ImGui::Begin(label.data(), open, mainFlags);

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
