#pragma once

#include <cstdint>
#include <cstring>
#include <set>
#include <string>
#include <string_view>

#include <imgui.h>

class WindowClass
{
public:
    constexpr static const char *function_names[] =
        {"unk", "sin(x)", "cos(x)", "exp(x)", "log(x)", "sqrt(x)"};

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
    WindowClass() : selected_functions({})
    {
        std::memset(user_input, 0, sizeof(user_input));
    };

    void Draw(std::string_view label);

private:
    static double evaluate_function(Function selected_function, double x);

private:
    char user_input[256];
    std::set<Function> selected_functions;
};

void render(WindowClass &window_class);
