#pragma once

#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include "miniaudio.h"

class WindowClass
{
public:
    WindowClass(ma_engine *engine_,
                ma_result *result_)
        : engine(engine_), result(result_)
    {
    };

    void Draw(std::string_view label);

private:
    void Load();

public:
    ma_engine *engine = nullptr;
    ma_result *result = nullptr;
};

void render(WindowClass &window_class);
