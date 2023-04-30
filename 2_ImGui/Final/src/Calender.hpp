#pragma once

#include <chrono>
#include <cstdint>
#include <fstream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <imgui.h>
#include <implot.h>

#include "WindowBase.hpp"

class Calender : public WindowBase
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

        void Serialize(std::ostream &out) const
        {
            const auto nameLength = static_cast<std::uint32_t>(name.size());
            out.write(reinterpret_cast<const char *>(&nameLength),
                      sizeof(nameLength));
            out.write(name.data(), name.size());
        }

        static Meeting Deserialize(std::istream &in)
        {
            auto meeting = Meeting{};
            auto nameLength = std::uint32_t{};
            in.read(reinterpret_cast<char *>(&nameLength), sizeof(nameLength));
            meeting.name.resize(nameLength);
            in.read(meeting.name.data(), nameLength);
            return meeting;
        }
    };

public:
    Calender(){};
    virtual ~Calender(){};
    void Draw(std::string_view label, bool *open = nullptr) final;

    void LoadMeetingsFromFile(const std::string_view filename);
    void SaveMeetingsToFile(const std::string_view filename) const;

private:
    void DrawCalendar();
    void DrawAddMeetingWindow();
    void DrawDateCombo(std::string_view label, int *day, int *month, int *year);
    void DrawMeetingsList();

    void UpdateSelectedDateVariables();

private:
    int selectedDay = 1;
    int selectedMonth = 1;
    int selectedYear = 2023;
    std::chrono::year_month_day selected_date;

    bool addMeetingWindowOpen = false;
    std::map<std::chrono::year_month_day, std::vector<Meeting>> meetings;

    float calendarFontScale = 1.25f;
};
