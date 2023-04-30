#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string_view>

#include <imgui.h>

#include <fmt/format.h>
#include <implot.h>

#include "FileExplorer.hpp"

void FileExplorer::Draw(std::string_view label, bool *open)
{
    ImGui::SetNextWindowPos(mainWindowPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(mainWindowSize, ImGuiCond_Always);

    ImGui::Begin(label.data(), open, mainWindowFlags);

    if (ImGui::Button("Go Up"))
    {
        if (currentPath.has_parent_path())
        {
            currentPath = currentPath.parent_path();
        }
    }

    ImGui::SameLine();

    ImGui::Text("Current directory: %s", currentPath.string().c_str());

    ImGui::Separator();

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

    ImGui::Separator();

    if (!selectedEntry.empty())
    {
        if (fs::is_directory(selectedEntry))
        {
            ImGui::Text("Selected dir: %s", selectedEntry.string().c_str());
        }
        else
        {
            ImGui::Text("Selected file: %s", selectedEntry.string().c_str());
        }

        if (fs::is_regular_file(selectedEntry))
        {
            if (ImGui::Button("Open"))
            {
                openFileWithDefaultEditor(selectedEntry);
            }

            ImGui::SameLine();
        }

        if (ImGui::Button("Rename"))
        {
            renameDialogOpen = true;
        }

        if (renameDialogOpen)
        {
            ImGui::OpenPopup("Rename File");
        }

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
            {
                renameDialogOpen = false;
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Delete"))
        {
            deleteDialogOpen = true;
        }

        if (deleteDialogOpen)
        {
            ImGui::OpenPopup("Delete File");
        }

        if (ImGui::BeginPopupModal("Delete File", &deleteDialogOpen))
        {
            ImGui::Text("Are you sure you want to delete %s?",
                        selectedEntry.filename().string().c_str());

            if (ImGui::Button("Yes"))
            {
                if (deleteFile(selectedEntry))
                {
                    selectedEntry.clear();
                }
                deleteDialogOpen = false;
            }

            ImGui::SameLine();

            if (ImGui::Button("No"))
            {
                deleteDialogOpen = false;
            }

            ImGui::EndPopup();
        }
    }

    ImGui::SameLine();

    static char extension_filter[16] = {"\0"};
    auto filtered_file_count = 0;
    ImGui::Text("Filter by extension");
    ImGui::SameLine();
    ImGui::InputText("###inFilter", extension_filter, sizeof(extension_filter));

    for (const auto &entry : fs::directory_iterator(currentPath))
    {
        if (!fs::is_regular_file(entry))
            continue;

        if (entry.path().extension().string() == extension_filter)
            filtered_file_count++;
    }

    ImGui::Text("Number of files: %d", filtered_file_count);

    ImGui::End();
}

void FileExplorer::openFileWithDefaultEditor(const fs::path &filePath)
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

bool FileExplorer::renameFile(const fs::path &oldPath, const fs::path &newPath)
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

bool FileExplorer::deleteFile(const fs::path &path)
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
