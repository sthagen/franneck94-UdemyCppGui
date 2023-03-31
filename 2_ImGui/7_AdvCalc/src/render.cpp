#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

void AdvancedCalc::Draw(std::string_view label)
{
    ImGui::Begin(label.data());
    ImGui::Combo("Function",
                 reinterpret_cast<int *>(&selected_function),
                 function_names,
                 IM_ARRAYSIZE(function_names));
    ImGui::SliderFloat("X min", &x_min, -500.0F, x_max - 1.0F);
    ImGui::SliderFloat("X max", &x_max, x_min + 1.0F, 500.0F);
    ImGui::End();

    const auto step = 0.1;
    const auto num_points =
        static_cast<std::size_t>(((x_max - x_min) / step) + 1);

    auto xs = std::vector<double>(num_points, 0.0F);
    auto ys = std::vector<double>(num_points, 0.0F);

    std::size_t i = 0;
    for (double x = x_min; x < x_max; x += step)
    {
        xs[i] = x;
        ys[i] = evaluate_function(selected_function, x);

        ++i;
    }

    const auto [min_y_it, max_y_it] = std::minmax_element(ys.begin(), ys.end());
    const auto min_y = *min_y_it;
    const auto max_y = *max_y_it;

    ImPlot::SetNextAxisLimits(ImAxis_X1, x_min, x_max);
    ImPlot::SetNextAxisLimits(ImAxis_Y1,
                              min_y - std::abs(min_y * 0.25F),
                              max_y + std::abs(max_y * 0.25F));
    if (xs.size() > 0 && ImPlot::BeginPlot("Function Plot", "x", "y"))
    {
        ImPlot::PlotScatter(
            function_names[static_cast<std::size_t>(selected_function)],
            xs.data(),
            ys.data(),
            xs.size());
        ImPlot::EndPlot();
    }
}

double AdvancedCalc::evaluate_function(Function selected_function, double x)
{
    switch (selected_function)
    {
    case Function::SIN:
        return std::sin(x);
    case Function::COS:
        return std::cos(x);
    case Function::TAN:
        return std::tan(x);
    case Function::EXP:
        return std::exp(x);
    case Function::LOG:
        return x > 0 ? std::log(x) : 0.0;
    case Function::SQRT:
        return x >= 0 ? std::sqrt(x) : 0.0;
    default:
        return 0.0;
    }
}

void render(AdvancedCalc &adv_calc)
{
    adv_calc.Draw("Calculator Tool");
}
