#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "miniaudio.h"
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Load()
{
    ma_engine_init(NULL, &engine);
    engineInitialized = true;
}

void WindowClass::Draw(std::string_view label)
{
    if (!engineInitialized)
        Load();

    if (isPlaying && !ma_sound_is_playing(&sound))
    {
        isPlaying = false;
    }

    ImGui::Begin(label.data());

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
                                    0,
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

void render(WindowClass &window_class)
{
    window_class.Draw("Media Player");
}
