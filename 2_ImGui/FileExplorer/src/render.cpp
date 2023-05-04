#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string_view>

#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>

#include "render.hpp"

void WindowClass::Draw(std::string_view label)
{
    constexpr static auto main_window_flags =
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;
    constexpr static auto main_window_size = ImVec2(1280.0F, 720.0F);
    constexpr static auto main_window_pos = ImVec2(0.0F, 0.0F);

    ImGui::SetNextWindowSize(main_window_size);
    ImGui::SetNextWindowPos(main_window_pos);

    ImGui::Begin(label.data(), nullptr, main_window_flags);

    DrawMenu();
    ImGui::Separator();
    DrawContent();
    ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 100.0F);
    ImGui::Separator();
    DrawActions();
    ImGui::Separator();
    DrawFilter();

    ImGui::End();
}

void WindowClass::DrawMenu()
{
    if (ImGui::Button("Go Up"))
    {
        if (currentPath.has_parent_path())
        {
            currentPath = currentPath.parent_path();
        }
    }

    ImGui::SameLine();

    ImGui::Text("Current directory: %s", currentPath.string().c_str());
}

void WindowClass::DrawContent()
{
    for (const auto &entry : fs::directory_iterator(currentPath))
    {
        const auto is_selected = entry.path() == selectedEntry;
        const auto is_directory = entry.is_directory();
        const auto is_file = entry.is_regular_file();
        auto entry_name = entry.path().filename().string();

        if (is_directory)
            entry_name = "[D] " + entry_name;
        if (is_file)
            entry_name = "[F] " + entry_name;

        if (ImGui::Selectable(entry_name.c_str(), is_selected))
        {
            if (is_directory)
            {
                currentPath /= entry.path().filename();
            }

            selectedEntry = entry.path();
        }
    }
}

void WindowClass::DrawActions()
{
    if (fs::is_directory(selectedEntry))
        ImGui::Text("Selected dir: %s", selectedEntry.string().c_str());
    else if (fs::is_regular_file(selectedEntry))
        ImGui::Text("Selected file: %s", selectedEntry.string().c_str());
    else
    {
        ImGui::Text("Selected file: n/a");
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha,
                            ImGui::GetStyle().Alpha * 0.0f);
        ImGui::Button("Non-clickable button");
        ImGui::PopStyleVar();
        return;
    }

    if (fs::is_regular_file(selectedEntry))
    {
        if (ImGui::Button("Open"))
            openFileWithDefaultEditor(selectedEntry);

        ImGui::SameLine();
    }

    if (ImGui::Button("Rename"))
        renameDialogOpen = true;

    if (renameDialogOpen)
        ImGui::OpenPopup("Rename File");

    if (ImGui::BeginPopupModal("Rename File", &renameDialogOpen))
    {
        static char buffer_name[512] = {'\0'};

        ImGui::Text("New name: ");
        ImGui::InputText("###NewName", buffer_name, sizeof(buffer_name));

        if (ImGui::Button("Rename"))
        {
            auto new_path = selectedEntry.parent_path() / buffer_name;
            if (renameFile(selectedEntry, new_path))
            {
                renameDialogOpen = false;
                selectedEntry = new_path;
                std::memset(buffer_name, 0, sizeof(buffer_name));
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            renameDialogOpen = false;

        ImGui::EndPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Delete"))
        deleteDialogOpen = true;

    if (deleteDialogOpen)
        ImGui::OpenPopup("Delete File");

    if (ImGui::BeginPopupModal("Delete File", &deleteDialogOpen))
    {
        ImGui::Text("Are you sure you want to delete %s?",
                    selectedEntry.filename().string().c_str());

        if (ImGui::Button("Yes"))
        {
            if (deleteFile(selectedEntry))
                selectedEntry.clear();
            deleteDialogOpen = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("No"))
            deleteDialogOpen = false;

        ImGui::EndPopup();
    }
}

void WindowClass::DrawFilter()
{
    static char extension_filter[16] = {"\0"};

    auto filtered_file_count = 0;
    ImGui::Text("Filter by extension");
    ImGui::SameLine();
    ImGui::InputText("###inFilter", extension_filter, sizeof(extension_filter));

    if (std::strlen(extension_filter) == 0)
        return;

    for (const auto &entry : fs::directory_iterator(currentPath))
    {
        if (!fs::is_regular_file(entry))
            continue;

        if (entry.path().extension().string() == extension_filter)
            filtered_file_count++;
    }

    ImGui::Text("Number of files: %d", filtered_file_count);
}

void WindowClass::openFileWithDefaultEditor(const fs::path &filePath)
{
#ifdef _WIN32
    const auto command = "start \"\" \"" + filePath.string() + "\"";
#elif __APPLE__
    const auto command = "open \"" + filePath.string() + "\"";
#else
    const auto command = "xdg-open \"" + filePath.string() + "\"";
#endif

    std::system(command.c_str());
}

bool WindowClass::renameFile(const fs::path &oldPath, const fs::path &newPath)
{
    try
    {
        fs::rename(oldPath, newPath);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

bool WindowClass::deleteFile(const fs::path &path)
{
    try
    {
        fs::remove(path);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void render(WindowClass &window_class)
{
    window_class.Draw("File Explorer Tool");
}
