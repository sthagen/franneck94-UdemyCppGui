#pragma once

#include <array>
#include <chrono>
#include <compare>
#include <fstream>
#include <map>
#include <string_view>
#include <vector>

#include <imgui.h>

class WindowClass
{
public:
    static constexpr auto months = std::array<std::string_view, 12>{
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
    };
    static constexpr auto minYear = 2000U;
    static constexpr auto maxYear = 2038U;

    struct Meeting
    {
        std::string name;

        void Serialize(std::ofstream &out) const;
        static Meeting Deserialize(std::ifstream &in);

        constexpr bool operator==(const Meeting &other) const
        {
            return name == other.name;
        }
    };

public:
    void Draw(std::string_view label);

    void LoadMeetingsFromFile(std::string_view filename);
    void SaveMeetingsToFile(std::string_view filename) const;

    void DrawDateCombo();
    void DrawCalender();
    void DrawAddMeetingWindow();
    void DrawMeetingList();
    void UpdateSelectedDateVariables();

private:
    int selectedDay = 1;
    int selectedMonth = 1;
    int selectedYear = 2023;
    std::chrono::year_month_day selectedDate;

    bool addMeetingWindowOpen = false;

    float calenderFontSize = 2.00f;

    std::map<std::chrono::year_month_day, std::vector<Meeting>> meetings;
};

void render(WindowClass &window_class);
