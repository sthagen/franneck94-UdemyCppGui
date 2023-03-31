#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <imgui.h>

class AdvancedCalc
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
    AdvancedCalc()
    {
        std::memset(user_input, 0, 256);
        selected_function = Function::NONE;
        x_min = -10.0F;
        x_max = 10.0F;
    };

    void Draw(std::string_view label);

private:
    double evaluate_function(Function selected_function, double x);

private:
    char user_input[256];
    Function selected_function;
    float x_min;
    float x_max;
};

void render(AdvancedCalc &adv_calc);
