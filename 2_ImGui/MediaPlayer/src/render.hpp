#pragma once

#include <string>
#include <string_view>

#include <imgui.h>

#include "miniaudio.h"
#include <implot.h>

class WindowClass
{
public:
    WindowClass() : engine({}), sound({}){};

    void Draw(std::string_view label);

private:
    void Load();

public:
    ma_engine engine;
    ma_sound sound;

    bool engineInitialized = false;
    bool soundInitialized = false;
    bool isPlaying = false;

    std::string audioFilePath =
        ("C:/Users/Jan/Documents/_LocalCoding/UdemyCppGui/2_ImGui/"
         "8_MediaPlayer/audio/_intro.mp3");
};

void render(WindowClass &window_class);
