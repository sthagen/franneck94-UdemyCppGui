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
    static constexpr auto num_points = 10'000;
    static constexpr auto x_min = -100.0f;
    static constexpr auto x_max = 100.0f;
    static const auto step = (std::abs(x_max) + std::abs(x_min)) / num_points;

    static auto xs = std::vector<double>(num_points, 0.0F);
    static auto ys = std::vector<double>(num_points, 0.0F);

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

    ImGui::End();

    ImGui::Begin("###plotWindow");

    if (selected_functions.size() == 0)
    {
        ImPlot::BeginPlot("ScatterPlot",
                          ImVec2(-1.0F, -1.0F),
                          ImPlotFlags_NoTitle);
        ImPlot::EndPlot();
        ImGui::End();

        return;
    }

    if (ImPlot::BeginPlot("###plotPlot"))
    {
        ImPlot::SetupAxes("x", "y", ImPlotAxisFlags_None, ImPlotAxisFlags_AutoFit);

        for (const auto &func : selected_functions)
        {
            double x = x_min;
            for (std::size_t i = 0; i < num_points; ++i)
            {
                xs[i] = x;
                const auto selected_function = func;
                ys[i] = evaluate_function(selected_function, x);
                x += step;
            }

            ImPlot::PlotLine(function_names[static_cast<std::size_t>(func)],
                             xs.data(),
                             ys.data(),
                             static_cast<int>(xs.size()));
        }
        ImPlot::EndPlot();
    }

    ImGui::End();
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
