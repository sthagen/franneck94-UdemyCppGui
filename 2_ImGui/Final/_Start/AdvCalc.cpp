#include <array>
#include <cmath>
#include <set>
#include <string_view>

#include <imgui.h>

#include <fmt/format.h>
#include <implot.h>

#include "AdvCalc.hpp"

void AdvCalc::Draw(std::string_view label)
{
    static constexpr auto num_points = 10'000;
    static constexpr auto x_min = -100.0;
    static constexpr auto x_max = 100.0;
    static const auto x_step = (std::abs(x_max) + std::abs(x_min)) / num_points;

    static auto xs = std::array<double, num_points>{};
    static auto ys = std::array<double, num_points>{};

    ImGui::Begin(label.data());

    for (const auto func_name : functionNames)
    {
        const auto curr_function = functionNameMapping(func_name);
        auto selected = selectedFunctions.contains(curr_function);
        if (ImGui::Checkbox(func_name.data(), &selected))
        {
            if (selected)
                selectedFunctions.insert(curr_function);
            else
                selectedFunctions.erase(curr_function);
        }
    }

    ImPlot::BeginPlot("###plot",
                      ImVec2(-1.0F, 720.0F - ImGui::GetCursorPosY()),
                      ImPlotFlags_NoTitle);

    if (selectedFunctions.size() == 0 ||
        (selectedFunctions.size() == 1 &&
         *selectedFunctions.begin() == Function::NONE))
    {
        ImPlot::EndPlot();
        ImGui::End();

        return;
    }

    for (const auto &function : selectedFunctions)
    {
        auto x = x_min;
        for (std::size_t i = 0; i < num_points; ++i)
        {
            xs[i] = x;
            ys[i] = evaluateFunction(function, x);
            x += x_step;
        }

        const auto plot_label =
            fmt::format("##function{}", static_cast<int>(function));
        ImPlot::PlotLine(plot_label.data(), xs.data(), ys.data(), num_points);
    }

    ImPlot::EndPlot();

    ImGui::End();
}

double AdvCalc::evaluateFunction(const Function function, const double x)
{
    switch (function)
    {
    case Function::SIN:
    {
        return std::sin(x);
    }
    case Function::COS:
    {
        return std::cos(x);
    }
    case Function::NONE:
    default:
        return 0.0;
    }
}

AdvCalc::Function AdvCalc::functionNameMapping(std::string_view function_name)
{
    if (std::string_view{"sin(x)"} == function_name)
        return AdvCalc::Function::SIN;

    if (std::string_view{"cos(x)"} == function_name)
        return AdvCalc::Function::COS;

    return AdvCalc::Function::NONE;
}
