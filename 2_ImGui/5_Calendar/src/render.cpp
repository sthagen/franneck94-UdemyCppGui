#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"

#include "render.hpp"

void Calendar::Draw(std::string_view title)
{
    ImGui::Begin(title.data());

    ImGui::Text("Select a date:");
    DrawDateCombo("##date", &selectedDay, &selectedMonth, &selectedYear);

    if (ImGui::Button("Add Meeting"))
    {
        addMeetingWindowOpen = true;
    }

    DrawCalendar();

    if (addMeetingWindowOpen)
    {
        DrawAddMeetingWindow();
    }

    ImGui::End();
}

void Calendar::DrawDateCombo(std::string_view label,
                             int *day,
                             int *month,
                             int *year)
{
    ImGui::PushItemWidth(50);

    if (ImGui::BeginCombo(fmt::format("##day{}", label).c_str(),
                          std::to_string(*day).c_str()))
    {
        for (std::uint32_t d = 1; d <= 31; ++d)
        {
            const auto curr_date =
                std::chrono::year_month_day(std::chrono::year(*year),
                                            std::chrono::month(*month),
                                            std::chrono::day(d));

            if (!curr_date.ok())
                break;

            if (ImGui::Selectable(std::to_string(d).c_str(), d == *day))
            {
                *day = d;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);

    if (ImGui::BeginCombo(fmt::format("##month{}", label).c_str(),
                          months[*month]))
    {
        for (std::uint32_t m = 1; m <= 12; ++m)
        {
            if (ImGui::Selectable(months[m - 1], m == *month))
            {
                *month = m;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(60);

    if (ImGui::BeginCombo(fmt::format("##year{}", label).c_str(),
                          std::to_string(*year).c_str()))
    {
        for (std::uint32_t y = 2023; y <= 2023; ++y)
        {
            if (ImGui::Selectable(std::to_string(y).c_str(), y == *year))
            {
                *year = y;
            }
        }
        ImGui::EndCombo();
    }
}

void Calendar::DrawCalendar()
{
    ImGui::BeginChild("Calendar",
                      ImVec2(ImGui::GetContentRegionAvail().x, -1.0f),
                      true);

    const auto curr_time =
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto todays_date = std::chrono::year_month_day{curr_time};

    for (std::uint32_t i = 1; i <= 12; ++i)
    {
        for (std::uint32_t j = 1; j <= 31; ++j)
        {
            const auto curr_date = std::chrono::year_month_day(
                std::chrono::year(todays_date.year()),
                std::chrono::month(i),
                std::chrono::day(j));
            if (!curr_date.ok())
                break;

            if (j != 1)
                ImGui::SameLine();
            if (j == 1)
                ImGui::Text(fmt::format("{:.3}", months[i - 1]).c_str());
            ImGui::SameLine();
            ImGui::PushID(j);

            ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            if (meetings.contains(i) && meetings[i].contains(j))
            {
                const auto hasMeeting = !meetings[i][j].empty();
                if (hasMeeting)
                    textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            }

            if (curr_date == todays_date)
                textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            if (selected_date == curr_date)
                textColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::Text(std::to_string(j).c_str());
            ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {
                selected_date =
                    std::chrono::year_month_day{std::chrono::year(selectedYear),
                                                std::chrono::month(i),
                                                std::chrono::day(j)};
            }

            ImGui::PopID();
        }
    }
    ImGui::EndChild();
}


void Calendar::DrawAddMeetingWindow()
{
    ImGui::Begin("Add Meeting",
                 &addMeetingWindowOpen,
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Add meeting to %s %d, %d",
                months[selectedMonth],
                selectedDay,
                selectedYear);
    ImGui::InputText("Meeting Name", &newMeetingName);

    if (ImGui::Button("Save"))
    {
        if (!newMeetingName.empty())
        {
            const auto newMeeting = Meeting{newMeetingName};
            meetings[selectedMonth][selectedDay].push_back(newMeeting);
            newMeetingName.clear();
        }
        addMeetingWindowOpen = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
        addMeetingWindowOpen = false;
    }

    ImGui::End();
}

void render(Calendar &calenar)
{
    calenar.Draw("Calendar Tool");
}
