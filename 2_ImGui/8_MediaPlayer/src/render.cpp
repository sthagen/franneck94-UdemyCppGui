#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include "miniaudio.h"

#include "render.hpp"

void WindowClass::Load()
{
    *result = ma_engine_init(NULL, engine);
    if (*result != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize audio engine.\n";
    }
}

void WindowClass::Draw(std::string_view label)
{
    static bool isPlaying = false;

    if (!engine || !result)
        return;

    ImGui::Begin(label.data());

    if (ImGui::Button(isPlaying ? "Pause" : "Play"))
    {
        if (isPlaying)
        {
            ma_engine_stop(engine);
        }
        else
        {
            Load();
            ma_engine_play_sound(
                engine,
                "C:/Users/Jan/Documents/_LocalCoding/UdemyCppGui/2_ImGui/"
                "8_MediaPlayer/audio/_intro.mp3",
                NULL);
        }
        isPlaying = !isPlaying;
    }


    ImGui::End();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Media Player");
}
