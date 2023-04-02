#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <fmt/format.h>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    ImGui::Begin(label.data());

    for (int i = 1; i < IM_ARRAYSIZE(function_names); ++i)
    {
        Function func = static_cast<Function>(i);
        bool selected = selected_functions.count(func) > 0;
        if (ImGui::Checkbox(function_names[i], &selected))
        {
            if (selected)
                selected_functions.insert(func);
            else
                selected_functions.erase(func);
        }
    }

    ImGui::SliderFloat("X min", &x_min, -500.0F, x_max - 1.0F);
    ImGui::SliderFloat("X max", &x_max, x_min + 1.0F, 500.0F);
    ImGui::End();

    if (selected_functions.size() == 0)
    {
        if (ImPlot::BeginPlot("ScatterPlot"))
        {
            ImPlot::EndPlot();
        }

        return;
    }

    const auto step = 0.05;
    const auto num_points =
        static_cast<std::size_t>(((x_max - x_min) / step) + 1);

    auto xs = std::vector<double>(num_points, 0.0F);
    auto ys = std::vector<double>(num_points, 0.0F);

    if (ImPlot::BeginPlot("plot"))
    {
        std::size_t func_idx = 0;

        for (const auto &func : selected_functions)
        {
            double x = x_min;
            for (std::size_t i = 0; i < num_points; ++i)
            {
                xs[i] = x;
                const auto selected_function =
                    func; // Set the current function to evaluate
                ys[i] = evaluate_function(selected_function, x);
                x += step;
            }

            if (func_idx == 0)
            {
                const auto [min_y_it, max_y_it] =
                    std::minmax_element(ys.begin(), ys.end());
                const auto min_y = *min_y_it;
                const auto max_y = *max_y_it;

                ImPlot::SetupAxisLimits(ImAxis_X1,
                                        x_min,
                                        x_max,
                                        ImPlotCond_Once);
                ImPlot::SetupAxisLimits(ImAxis_Y1,
                                        min_y - std::abs(min_y * 0.5F),
                                        max_y + std::abs(max_y * 0.5F),
                                        ImPlotCond_Once);
            }

            ImPlot::PlotLine(function_names[static_cast<std::size_t>(func)],
                             xs.data(),
                             ys.data(),
                             static_cast<int>(xs.size()));

            ++func_idx;
        }
    }
    ImPlot::EndPlot();
}

double WindowClass::evaluate_function(Function selected_function, double x)
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

void render(WindowClass &window_class)
{
    window_class.Draw("Calculator Tool");
}
