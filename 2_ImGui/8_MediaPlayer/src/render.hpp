#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <SFML/Audio.hpp>
#include <imgui.h>

class MediaPlayer
{
public:
    MediaPlayer() = default;
    ~MediaPlayer() = default;

    void loadFile(const std::string &filename);

    void play();
    void pause();
    void stop();

    void Draw(std::string_view label);

private:
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
};

void render(MediaPlayer &media_player);
