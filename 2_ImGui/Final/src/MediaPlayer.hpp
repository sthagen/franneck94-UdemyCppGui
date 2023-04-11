#pragma once

#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "miniaudio.h"

#include "BaseView.hpp"

class MediaPlayer : public BaseView
{
public:
    MediaPlayer()
        : engine({}){};
    virtual ~MediaPlayer(){};

    void Draw(std::string_view label, bool *open = nullptr) override;

private:
    void Load();

public:
    ma_engine engine;
    ma_sound sound;

    bool engineInitialized = false;
    bool soundInitialized = false;
    bool isPlaying = false;

    std::string audioFilePath =
        ("C:/Users/Jan/Documents/_LocalCoding/UdemyCppGui/2_ImGui/Final/audio/_intro.mp3");
};
