#pragma once

#include <array>
#include <set>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "BaseView.hpp"

class AdvCalc : public BaseView
{
public:
    constexpr static auto functionNames =
        std::array<std::string_view, 3>{"unk", "sin(x)", "cos(x)"};

    enum class Function
    {
        NONE,
        SIN,
        COS,
    };

public:
    AdvCalc() : selectedFunctions({}){};
    virtual ~AdvCalc() = default;

    void Draw(std::string_view label, bool *open = NULL) override;
    Function functionNameMapping(std::string_view function_name);
    double evaluateFunction(const Function function, const double x);

public:
    std::set<Function> selectedFunctions;
};
