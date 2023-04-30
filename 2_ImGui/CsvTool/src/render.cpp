#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(window_size);
    ImGui::SetNextWindowPos(window_pos);

    ImGui::Begin(label.data(), nullptr, window_flags);

    DrawSizeButtons();
    DrawIoButtons();
    DrawTable();

    ImGui::End();
}

void WindowClass::DrawSizeButtons()
{
    auto user_added_rows = false;
    auto user_dropped_rows = false;
    auto user_added_cols = false;
    auto user_dropped_cols = false;

    auto slider_rows = numRows;
    auto slider_cols = numCols;

    ImGui::Text("Num Rows: ");
    ImGui::SameLine();
    if (ImGui::SliderInt("##numRows", &slider_rows, 0, 30))
    {
        user_added_rows = slider_rows > numRows ? true : false;
        user_dropped_rows = !user_dropped_rows;

        numRows = slider_rows;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Row") && numRows < 30)
    {
        ++numRows;
        user_added_rows = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Drop Row") && numRows > 0)
    {
        --numRows;
        user_dropped_rows = true;
    }

    ImGui::Text("Num Cols: ");
    ImGui::SameLine();
    if (ImGui::SliderInt("##numCols", &slider_cols, 0, 8))
    {
        user_added_cols = slider_cols > numCols ? true : false;
        user_dropped_cols = !user_dropped_cols;

        numCols = slider_cols;
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Col") && numCols < 8)
    {
        ++numCols;
        user_added_cols = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Drop Col") && numCols > 0)
    {
        --numCols;
        user_dropped_cols = true;
    }

    if (user_added_rows)
    {
        for (auto row = static_cast<std::int32_t>(data.size()); row < numRows;
             ++row)
        {
            data.push_back(std::vector<float>(numCols, 0.0F));
        }
    }
    else if (user_added_cols)
    {
        for (std::int32_t row = 0; row < numRows; ++row)
        {
            for (auto col = static_cast<std::int32_t>(data[row].size());
                 col < numCols;
                 ++col)
            {
                data[row].push_back(0.0F);
            }
        }
    }
    else if (user_dropped_rows)
    {
        for (auto row = static_cast<std::int32_t>(data.size()); row > numRows;
             --row)
        {
            data.pop_back();
        }
    }
    else if (user_dropped_cols)
    {
        for (std::int32_t row = 0; row < numRows; ++row)
        {
            for (auto col = static_cast<std::int32_t>(data[row].size());
                 col > numCols;
                 --col)
            {
                data[row].pop_back();
            }
        }
    }
}

void WindowClass::DrawIoButtons()
{
    static char filenameBuffer[512] = "test.csv";

    ImGui::Separator();

    if (ImGui::Button("Save"))
        ImGui::OpenPopup("Save File");

    ImGui::SameLine();

    if (ImGui::Button("Load"))
        ImGui::OpenPopup("Load File");

    ImGui::SameLine();

    if (ImGui::Button("Clear"))
    {
        data.clear();
        numRows = 0;
        numCols = 0;
    }

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popUpSize.x / 2.0F,
               ImGui::GetIO().DisplaySize.y / 2.0F - popUpSize.y / 2.0F));
    if (ImGui::BeginPopupModal("Save File", nullptr, popUpFlags))
    {
        ImGui::InputText("File", filenameBuffer, sizeof(filenameBuffer));

        if (ImGui::Button("Save"))
        {
            SaveToFile(filenameBuffer);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popUpSize.x / 2.0F,
               ImGui::GetIO().DisplaySize.y / 2.0F - popUpSize.y / 2.0F));
    if (ImGui::BeginPopupModal("Load File", nullptr, popUpFlags))
    {
        ImGui::InputText("File", filenameBuffer, sizeof(filenameBuffer));

        if (ImGui::Button("Load"))
        {
            LoadFromFile(filenameBuffer);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::Separator();
}

void WindowClass::DrawTable()
{
    constexpr static auto table_flags =
        (ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuter);

    static char buffer[512] = {'\0'};
    static auto row_clicked = 0;
    static auto col_clicked = 0;

    if (numCols == 0)
        return;

    ImGui::BeginTable("Table", numCols, table_flags);

    for (std::int32_t col = 0; col < numCols; ++col)
    {
        ImGui::TableSetupColumn(fmt::format("{}", 'A' + col).data(),
                                ImGuiTableColumnFlags_WidthFixed,
                                1280.0F / static_cast<float>(numCols));
    }

    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
    for (std::int32_t col = 0; col < numCols; ++col)
        PlotCellValue("%c", 'A' + col);

    for (std::int32_t row = 0; row < numRows; ++row)
    {
        for (std::int32_t col = 0; col < numCols; ++col)
        {
            PlotCellValue("%f", data[row][col]);
            if (ImGui::IsItemClicked())
            {
                ImGui::OpenPopup("Change Value");
                row_clicked = row;
                col_clicked = col;
            }
            else if (ImGui::IsItemHovered())
            {
                ImGui::SetTooltip("Cell: (%d, %d)", row, col);
            }
        }

        ImGui::TableNextRow();
    }

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popUpSize.x / 2.0F,
               ImGui::GetIO().DisplaySize.y / 2.0F - popUpSize.y / 2.0F));
    if (ImGui::BeginPopupModal("Change Value", nullptr, popUpFlags))
    {
        ImGui::InputText(
            fmt::format("##{}_{}", row_clicked, col_clicked).data(),
            buffer,
            sizeof(buffer));

        if (ImGui::Button("Save"))
        {
            data[row_clicked][col_clicked] = std::stof(buffer);

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::EndTable();
}

void WindowClass::SaveToFile(std::string_view filename)
{
    auto out = std::ofstream{filename.data()};

    if (!out.is_open())
        return;

    for (std::int32_t row = 0; row < numRows; ++row)
    {
        for (std::int32_t col = 0; col < numCols; ++col)
        {
            out << data[row][col];
            out << ',';
        }

        out << '\n';
    }

    out.close();
}

void WindowClass::LoadFromFile(std::string_view filename)
{
    auto in = std::ifstream{filename.data()};

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
}

template <typename T>
void WindowClass::PlotCellValue(std::string_view formatter, const T value)
{
    ImGui::TableNextColumn();
    ImGui::Text(formatter.data(), value);
}

void render(WindowClass &window_obj)
{
    window_obj.Draw("Label");
}
