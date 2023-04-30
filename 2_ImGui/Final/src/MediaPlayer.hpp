#pragma once

#include <string>
#include <string_view>

#include <imgui.h>

#include "miniaudio.h"
#include <implot.h>

#include "WindowBase.hpp"

class MediaPlayer : public WindowBase
{
public:
    MediaPlayer() : engine({}){};
    virtual ~MediaPlayer(){};

    void Draw(std::string_view label, bool *open = nullptr) final;

private:
    void InitEngine();

public:
    ma_engine engine;
    ma_sound sound;

    bool engineInitialized = false;
    bool soundInitialized = false;
    bool isPlaying = false;

    std::string audioFilePath = ("C:/Users/Jan/Documents/_LocalCoding/"
                                 "UdemyCppGui/2_ImGui/Final/audio/_intro.mp3");
};
