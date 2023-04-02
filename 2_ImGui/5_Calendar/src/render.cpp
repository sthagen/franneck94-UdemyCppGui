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

void WindowClass::Draw(std::string_view title)
{
    ImGui::Begin(title.data(),
                 NULL,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);

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


    DrawMeetingsList();

    ImGui::End();
}

void WindowClass::DrawDateCombo(std::string_view label,
                                int *day,
                                int *month,
                                int *year)
{
    ImGui::PushItemWidth(50);

    if (ImGui::BeginCombo(fmt::format("##day{}", label).c_str(),
                          std::to_string(*day).c_str()))
    {
        for (std::int32_t day_idx = 1; day_idx <= 31; ++day_idx)
        {
            const auto curr_date =
                std::chrono::year_month_day(std::chrono::year(*year),
                                            std::chrono::month(*month),
                                            std::chrono::day(day_idx));

            if (!curr_date.ok())
                break;

            if (ImGui::Selectable(std::to_string(day_idx).c_str(),
                                  day_idx == *day))
            {
                *day = day_idx;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);

    if (ImGui::BeginCombo(fmt::format("##month{}", label).c_str(),
                          months[(*month) - 1]))
    {
        for (std::int32_t month_idx = 1; month_idx <= 12; ++month_idx)
        {
            if (ImGui::Selectable(months[month_idx - 1], month_idx == *month))
            {
                *month = month_idx;
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(60);

    if (ImGui::BeginCombo(fmt::format("##year{}", label).c_str(),
                          std::to_string(*year).c_str()))
    {
        for (std::int32_t y = 2023; y <= 2023; ++y)
        {
            if (ImGui::Selectable(std::to_string(y).c_str(), y == *year))
            {
                *year = y;
            }
        }
        ImGui::EndCombo();
    }
}

void WindowClass::DrawMeetingsList()
{
    if (meetings.contains(selected_date))
    {
        ImGui::Separator();
        ImGui::Text("Meetings on %d.%s.%d:",
                    selectedDay,
                    months[selectedMonth - 1],
                    selectedYear);

        const auto &meetingList = meetings[selected_date];

        if (meetingList.empty())
        {
            ImGui::Text("No meetings scheduled for this day.");
        }
        else
        {
            for (const auto &meeting : meetingList)
            {
                ImGui::BulletText("%s", meeting.name.c_str());
            }
        }
    }
    else
    {
        ImGui::Text("No meetings for that day!");
    }
}


void WindowClass::DrawCalendar()
{
    ImGui::BeginChild("WindowClass",
                      ImVec2(ImGui::GetContentRegionAvail().x,
                             ImGui::GetContentRegionAvail().y - 50.0F),
                      true);

    const auto curr_time =
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto todays_date = std::chrono::year_month_day{curr_time};

    float originalFontScale = ImGui::GetFontSize();
    ImGui::SetWindowFontScale(calendarFontScale);

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

            if (meetings.contains(curr_date))
                textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // red
            if (curr_date == todays_date)
                textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // green
            if (selected_date == curr_date)
                textColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f); // blue

            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::Text(std::to_string(j).c_str());
            ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {
                selected_date =
                    std::chrono::year_month_day{std::chrono::year(selectedYear),
                                                std::chrono::month(i),
                                                std::chrono::day(j)};
                UpdateSelectedDateVariables();
            }

            ImGui::PopID();
        }
    }

    ImGui::SetWindowFontScale(originalFontScale);

    ImGui::EndChild();
}


void WindowClass::DrawAddMeetingWindow()
{
    static char newMeetingName[128] = "output.txt";

    ImGui::Begin("Add Meeting",
                 &addMeetingWindowOpen,
                 ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Add meeting to %d.%s.%d",
                selectedDay,
                months[selectedMonth - 1],
                selectedYear);
    ImGui::InputText("Meeting Name", newMeetingName, sizeof(newMeetingName));

    if (ImGui::Button("Save"))
    {
        const auto newMeeting = Meeting{newMeetingName};
        meetings[selected_date].push_back(newMeeting);
        std::memset(newMeetingName, 0, sizeof(newMeetingName));
        addMeetingWindowOpen = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
    {
        addMeetingWindowOpen = false;
    }

    ImGui::End();
}

void WindowClass::UpdateSelectedDateVariables()
{
    selectedDay = static_cast<int>(selected_date.day().operator unsigned int());
    selectedMonth =
        static_cast<int>(selected_date.month().operator unsigned int());
    selectedYear = static_cast<int>(selected_date.year());
}

void render(WindowClass &window_class)
{
    window_class.Draw("WindowClass Tool");
}
