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
    ma_engine_init(NULL, &engine);
    engineInitialized = true;
}


void WindowClass::Draw(std::string_view label)
{
    if (!engineInitialized)
        Load();

    ImGui::Begin(label.data());

    ImGui::Text("Audio File Path:");
    ImGui::SameLine();
    ImGui::InputText("##AudioFilePath", &audioFilePath);

    if (ImGui::Button(isPlaying ? "Stop" : "Play"))
    {
        if (isPlaying)
        {
            ma_engine_stop(&engine);
            engineInitialized = false;
            isPlaying = false;
        }
        else
        {
            ma_engine_play_sound(&engine, audioFilePath.data(), NULL);
            isPlaying = true;
        }
    }

    ImGui::End();
}

void render(WindowClass &window_class)
{
    window_class.Draw("Media Player");
}
