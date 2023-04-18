#pragma once

#include <algorithm>
#include <filesystem>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include <SimpleIni.h>

namespace fs = std::filesystem;

class WindowBase
{
public:
    constexpr static auto fullscreenFlags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
    constexpr static auto rootPos = ImVec2(0.0F, 0.0F);
    constexpr static auto fullscreenSize = ImVec2(1280.0F, 720.0F);

public:
    WindowBase(){};
    virtual ~WindowBase(){};

    void SettingsMenuBar();
    void LoadTheme();
    void SaveTheme();

    virtual void Draw(std::string_view label, bool *open = nullptr) = 0;

protected:
    void DrawColorsSettings(bool *open = nullptr);
    static ImGuiStyle DefaultStyleColors();

    CSimpleIniA ini = CSimpleIniA();
};
