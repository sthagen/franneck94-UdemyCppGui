#pragma once

#include <cstdint>
#include <map>
#include <string_view>
#include <vector>

#include <imgui.h>

class WindowClass
{
public:
    static constexpr const char *months[12] = {"January",
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
                                               "December"};

    struct Meeting
    {
        std::string name;
    };

public:
    WindowClass() = default;

    void Draw(std::string_view title);

private:
    void DrawCalendar();
    void DrawAddMeetingWindow();
    void DrawDateCombo(std::string_view label, int *day, int *month, int *year);
    void DrawMeetingsList();

private:
    int selectedDay = 1;
    int selectedMonth = 1;
    int selectedYear = 2023;
    std::chrono::year_month_day selected_date;

    bool addMeetingWindowOpen = false;
    std::map<int, std::map<int, std::vector<Meeting>>> meetings;
};

void render(WindowClass &window_class);
