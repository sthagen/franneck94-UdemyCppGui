#include <algorithm>
#include <chrono>
#include <map>
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

    DrawDateCombo();
    ImGui::Separator();
    DrawCalender();
    ImGui::Separator();
    DrawMeetingList();

    if (addMeetingWindowOpen)
        DrawAddMeetingWindow();

    ImGui::End();
}

void WindowClass::DrawCalender()
{
    const auto child_size = ImVec2(ImGui::GetContentRegionAvail().x, 400.0F);

    ImGui::BeginChild("###calender", child_size, false);

    const auto original_font_size = ImGui::GetFontSize();
    ImGui::SetWindowFontScale(calenderFontSize);

    const auto curr_time =
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    const auto todays_date = std::chrono::year_month_day(curr_time);

    const auto y = selectedYear;
    for (std::int32_t m = 1; m <= 12; m++)
    {
        for (std::int32_t d = 1; d <= 31; d++)
        {
            const auto curr_date =
                std::chrono::year_month_day(std::chrono::year(y),
                                            std::chrono::month(m),
                                            std::chrono::day(d));

            if (!curr_date.ok())
                break;

            if (d != 1)
                ImGui::SameLine();
            if (d == 1)
                ImGui::Text("%s", fmt::format("{:.3}", months[m - 1]).data());
            ImGui::SameLine();

            auto textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            if (curr_date == todays_date)
                textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0F);
            else if (curr_date == selectedDate)
                textColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0F);
            else if (meetings.contains(curr_date))
                textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0F);

            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::Text("%s", std::to_string(d).data());
            ImGui::PopStyleColor();

            if (ImGui::IsItemClicked())
            {
                selectedDate = curr_date;
                UpdateSelectedDateVariables();
            }
        }
    }

    ImGui::SetWindowFontScale(original_font_size);

    ImGui::EndChild();
}

void WindowClass::UpdateSelectedDateVariables()
{
    selectedDay = static_cast<int>(selectedDate.day().operator unsigned int());
    selectedMonth =
        static_cast<int>(selectedDate.month().operator unsigned int());
    selectedYear = static_cast<int>(selectedDate.year());
}

void WindowClass::DrawAddMeetingWindow()
{
    constexpr static auto window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto popUpSize = ImVec2(300.0F, 100.0F);

    static char meeting_name_buffer[128] = "...";

    ImGui::SetNextWindowSize(popUpSize);
    ImGui::SetNextWindowPos(
        ImVec2(ImGui::GetIO().DisplaySize.x / 2.0F - popUpSize.x / 2.0F,
               ImGui::GetIO().DisplaySize.y / 2.0F - popUpSize.y / 2.0F));
    ImGui::Begin("###addMeeting", &addMeetingWindowOpen, window_flags);

    ImGui::Text("Add meeting to %d.%s.%d",
                selectedDay,
                months[selectedMonth - 1].data(),
                selectedYear);

    ImGui::InputText("Meeting Name",
                     meeting_name_buffer,
                     sizeof(meeting_name_buffer));

    if (ImGui::Button("Save"))
    {
        meetings[selectedDate].push_back(Meeting{meeting_name_buffer});
        std::memset(meeting_name_buffer, 0, sizeof(meeting_name_buffer));
        addMeetingWindowOpen = false;
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
    {
        addMeetingWindowOpen = false;
    }

    ImGui::End();
}

void WindowClass::DrawMeetingList()
{
    ImGui::Separator();

    if (!meetings.contains(selectedDate))
    {
        ImGui::Text("No meetings at all.");
        return;
    }

    ImGui::Text("Meetins on %d.%s.%d: ",
                selectedDay,
                months[selectedMonth - 1].data(),
                selectedYear);

    if (meetings[selectedDate].empty())
    {
        ImGui::Text("No meetings for this day.");
        return;
    }

    for (const auto &meeting : meetings[selectedDate])
    {
        ImGui::BulletText("%s", meeting.name.data());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            std::erase(meetings[selectedDate], meeting);
            return;
        }
    }
}

void WindowClass::DrawDateCombo()
{
    ImGui::Text("Select a date:");

    ImGui::PushItemWidth(50);

    if (ImGui::BeginCombo("##day", std::to_string(selectedDay).data()))
    {
        for (std::int32_t day_idx = 1; day_idx <= 31; ++day_idx)
        {
            const auto curr_date =
                std::chrono::year_month_day(std::chrono::year(selectedYear),
                                            std::chrono::month(selectedMonth),
                                            std::chrono::day(day_idx));

            if (!curr_date.ok())
                break;

            if (ImGui::Selectable(std::to_string(day_idx).data(),
                                  day_idx == selectedDay))
            {
                selectedDay = day_idx;
                selectedDate = curr_date;
            }
        }

        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(100);

    if (ImGui::BeginCombo("##month", months[selectedMonth - 1].data()))
    {
        for (std::int32_t month_idx = 1; month_idx <= 12; ++month_idx)
        {
            if (ImGui::Selectable(months[month_idx - 1].data(),
                                  month_idx == selectedMonth))
            {
                selectedMonth = month_idx;
                selectedDate =
                    std::chrono::year_month_day(std::chrono::year(selectedYear),
                                                std::chrono::month(month_idx),
                                                std::chrono::day(selectedDay));
            }
        }

        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(60);

    if (ImGui::BeginCombo("##year", std::to_string(selectedYear).data()))
    {
        for (std::int32_t year_idx = minYear; year_idx <= maxYear; ++year_idx)
        {
            const auto is_selected = (year_idx == selectedYear);
            if (ImGui::Selectable(std::to_string(year_idx).data(), is_selected))
            {
                selectedYear = year_idx;
                selectedDate = std::chrono::year_month_day(
                    std::chrono::year(year_idx),
                    std::chrono::month(selectedMonth),
                    std::chrono::day(selectedDay));
            }
            if (is_selected)
                ImGui::SetScrollHereY();
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Add Meeting"))
        addMeetingWindowOpen = true;
}

void WindowClass::SaveMeetingsToFile(std::string_view filename) const
{
    auto out = std::ofstream(filename.data(), std::ios::binary);

    if (!out.is_open())
        return;

    const auto num_meetings = meetings.size();
    out.write(reinterpret_cast<const char *>(&num_meetings),
              sizeof(num_meetings));

    for (const auto &[date, meeting_vec] : meetings)
    {
        out.write(reinterpret_cast<const char *>(&date), sizeof(date));

        const auto num_meetings_on_date = meeting_vec.size();
        out.write(reinterpret_cast<const char *>(&num_meetings_on_date),
                  sizeof(num_meetings_on_date));

        for (const auto &meeting : meeting_vec)
            meeting.Serialize(out);
    }

    out.close();
}

void WindowClass::LoadMeetingsFromFile(std::string_view filename)
{
    auto in = std::ifstream(filename.data(), std::ios::binary);

    if (!in.is_open())
        return;

    auto num_meetings = std::size_t{};
    in.read(reinterpret_cast<char *>(&num_meetings), sizeof(num_meetings));

    for (std::size_t i = 0; i < num_meetings; ++i)
    {
        auto date = std::chrono::year_month_day{};
        in.read(reinterpret_cast<char *>(&date), sizeof(date));

        auto num_meetings_on_date = std::size_t{};
        in.read(reinterpret_cast<char *>(&num_meetings_on_date),
                sizeof(num_meetings_on_date));

        for (std::size_t j = 0; j < num_meetings_on_date; ++j)
        {
            auto meeting = Meeting::Deserialize(in);
            meetings[date].push_back(meeting);
        }
    }

    in.close();
}

void WindowClass::Meeting::Serialize(std::ofstream &out) const
{
    const auto name_length = name.size();

    out.write(reinterpret_cast<const char *>(&name_length),
              sizeof(name_length));
    out.write(name.data(), static_cast<std::streamsize>(name.size()));
}

WindowClass::Meeting WindowClass::Meeting::Deserialize(std::ifstream &in)
{
    auto meeting = Meeting{};
    auto name_length = std::size_t{};

    in.read(reinterpret_cast<char *>(&name_length), sizeof(name_length));
    meeting.name.resize(name_length);
    in.read(meeting.name.data(), static_cast<std::streamsize>(name_length));

    return meeting;
}

void render(WindowClass &window_class)
{
    window_class.Draw("Calender Tool");
}
