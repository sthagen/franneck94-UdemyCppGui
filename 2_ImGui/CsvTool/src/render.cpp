#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "imgui.h"
#include "implot.h"
#include <fmt/format.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        (ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
         ImGuiWindowFlags_NoResize);
    constexpr static auto table_flags =
        (ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter);

    static char saveFilenameBuffer[512] = "test.csv";
    static char loadFilenameBuffer[512] = "test.csv";

    bool userAddedRows = false;
    bool userDroppedRows = false;
    bool userAddedCols = false;
    bool userDroppedCols = false;
    std::int32_t slider_rows = numRows;
    std::int32_t slider_cols = numCols;

    ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F));
    ImGui::SetNextWindowSize(ImVec2(1280.0F, 720.0F));

    ImGui::Begin(label.data(), nullptr, window_flags);

    ImGui::Text("Num Rows: ");
    ImGui::SameLine();
    if (ImGui::SliderInt("##Num Rows", &slider_rows, 0, 30))
    {
        userAddedRows = slider_rows > numRows ? true : false;
        userDroppedRows = !userAddedRows;
        numRows = slider_rows;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Row"))
    {
        ++numRows;
        userAddedRows = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Drop Row"))
    {
        --numRows;
        userDroppedRows = true;
    }

    ImGui::Text("Num Cols: ");
    ImGui::SameLine();
    if (ImGui::SliderInt("##Num Cols", &slider_cols, 0, 8))
    {
        userAddedCols = slider_cols > numCols ? true : false;
        userDroppedCols = !userAddedCols;
        numCols = slider_cols;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Col"))
    {
        userAddedCols = true;
        ++numCols;
    }
    ImGui::SameLine();
    if (ImGui::Button("Drop Col"))
    {
        userDroppedCols = true;
        --numCols;
    }

    if (userAddedRows)
    {
        for (auto row = data.size(); row < numRows; ++row)
        {
            data.push_back(std::vector<float>(numCols, 0));
        }
    }
    else if (userAddedCols)
    {
        for (std::int32_t row = 0; row < numRows; ++row)
        {
            data[row].push_back({});
        }
    }

    if (userAddedCols || userDroppedCols)
    {
        SetColNames();
    }

    ImGui::Separator();

    if (ImGui::Button("Save"))
    {
        ImGui::OpenPopup("Save File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Load"))
    {
        ImGui::OpenPopup("Load File");
    }

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        data.clear();
        colNames.clear();
        numCols = 0;
        numRows = 0;
    }

    if (ImGui::BeginPopupModal("Save File"))
    {
        ImGui::InputText("Filename",
                         saveFilenameBuffer,
                         sizeof(saveFilenameBuffer));

        if (ImGui::Button("Save", ImVec2(120.0F, 0.0F)))
        {
            SaveToFile(saveFilenameBuffer);
            currentFilename = saveFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120.0F, 0.0F)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    if (ImGui::BeginPopupModal("Load File"))
    {
        ImGui::InputText("Filename",
                         loadFilenameBuffer,
                         sizeof(loadFilenameBuffer));

        if (ImGui::Button("Load", ImVec2(120.0F, 0.0F)))
        {
            LoadFromFile(loadFilenameBuffer);
            currentFilename = loadFilenameBuffer;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(120.0F, 0.0F)))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::Separator();

    if (numCols > 0 && ImGui::BeginTable("Table", numCols, table_flags))
    {
        for (const auto col_name : colNames)
        {
            ImGui::TableSetupColumn(fmt::format("{}", col_name).data(),
                                    ImGuiTableColumnFlags_WidthFixed,
                                    1280.0F / numCols);
        }
        ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
        for (const auto col_name : colNames)
            PlotCellValue("%c", col_name);
        ImGui::TableNextRow();

        static char buffer[512] = {'\0'};

        for (std::int32_t row = 0; row < numRows; ++row)
        {
            for (std::int32_t col = 0; col < numCols; ++col)
            {
                PlotCellValue("%f", data[row][col]);
                if (ImGui::IsItemClicked())
                    ImGui::OpenPopup("Change Value");
                else if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Cell: (%d, %d)", row, col);

                if (ImGui::BeginPopupModal("Change Value"))
                {
                    ImGui::InputText(fmt::format("##{}_{}", row, col).data(),
                                     buffer,
                                     sizeof(buffer));
                    if (ImGui::Button("Save"))
                    {
                        const auto value = std::stof(buffer);
                        data[row][col] = value;

                        ImGui::CloseCurrentPopup();
                    }

                    ImGui::EndPopup();
                }
            }

            ImGui::TableNextRow();
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

template <typename T>
void WindowClass::PlotCellValue(std::string_view formatter, const T &value)
{
    ImGui::TableNextColumn();
    ImGui::Text(formatter.data(), value);
}

void WindowClass::SaveToFile(std::string_view filename)
{
    auto out = std::ofstream(filename.data());

    if (!out.is_open())
        return;

    for (std::int32_t row = 0; row < numRows; ++row)
    {
        for (std::int32_t col = 0; col < numCols; ++col)
        {
            const auto str_data = std::to_string(data[row][col]);
            out.write(str_data.data(), str_data.size());
            out << ',';
        }
        out << '\n';
    }

    out.close();
}

void WindowClass::LoadFromFile(std::string_view filename)
{
    auto in = std::ifstream(filename.data());

    if (!in.is_open())
        return;

    data.clear();

    auto line = std::string{};
    auto num_rows = 0;

    while (std::getline(in, line))
    {
        auto ss = std::istringstream(line);
        auto row = std::vector<float>{};

        auto value = std::string{};
        while (std::getline(ss, value, ','))
        {
            row.push_back(std::stof(value));
        }

        data.push_back(row);

        ++num_rows;
    }

    in.close();

    numRows = num_rows;
    if (num_rows > 0)
        numCols = static_cast<std::int32_t>(data[0].size());
    else
        numCols = 0;
    SetColNames();

    in.close();
}

void WindowClass::SetColNames()
{
    colNames.clear();
    for (std::int8_t col = 0; col < numCols; ++col)
        colNames.push_back('A' + col);
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
