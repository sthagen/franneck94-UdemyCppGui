#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <string_view>

#include <imgui.h>

class WindowClass
{
public:
    constexpr static const char *function_names[7] =
        {"unk", "sin(x)", "cos(x)", "tan(x)", "exp(x)", "log(x)", "sqrt(x)"};

    enum class Function
    {
        NONE,
        SIN,
        COS,
        TAN,
        EXP,
        LOG,
        SQRT
    };

public:
    WindowClass()
    {
        std::memset(user_input, 0, 256);
        selected_functions = {};
        x_min = -10.0F;
        x_max = 10.0F;
    };

    void Draw(std::string_view label);

private:
    static double evaluate_function(Function selected_function, double x);

private:
    char user_input[256];
    std::set<Function> selected_functions;
    float x_min;
    float x_max;
};

void render(WindowClass &window_class);
