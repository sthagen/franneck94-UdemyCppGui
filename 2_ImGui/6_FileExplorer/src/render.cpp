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

void FileExplorer::Draw(std::string_view label)
{
    static fs::path selectedEntry;

    ImGui::Begin(label.data());

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

    try
    {
        for (const auto &entry : fs::directory_iterator(currentPath))
        {
            auto entryName = entry.path().filename().string();
            const auto isDirectory = entry.is_directory();
            bool isSelected = entry.path() == selectedEntry;

            if (isDirectory)
            {
                entryName = "[D] " + entryName;
            }

            if (ImGui::Selectable(entryName.c_str(), isSelected))
            {
                if (isDirectory)
                {
                    currentPath /= entry.path().filename();
                }

                selectedEntry = entry.path();
            }
        }

        ImGui::Separator();
    }
    catch (const std::exception &e)
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error: %s", e.what());
    }

    if (!selectedEntry.empty())
    {
        ImGui::Text("Selected file: %s", selectedEntry.string().c_str());

        if (fs::is_regular_file(selectedEntry) && ImGui::Button("Open"))
        {
            openFileWithDefaultEditor(selectedEntry);
        }

        ImGui::SameLine();
        if (ImGui::Button("Rename"))
        {
            renameDialogOpen = true;
            strncpy(renameBuffer,
                    selectedEntry.filename().string().c_str(),
                    sizeof(renameBuffer) - 1);
        }

        if (renameDialogOpen)
        {
            ImGui::OpenPopup("Rename File");
        }
        if (ImGui::BeginPopupModal("Rename File", &renameDialogOpen))
        {
            ImGui::Text("New name:");
            ImGui::InputText("##NewName", renameBuffer, sizeof(renameBuffer));
            if (ImGui::Button("Rename"))
            {
                fs::path newPath = selectedEntry.parent_path() / renameBuffer;
                if (renameFile(selectedEntry, newPath))
                {
                    selectedEntry = newPath;
                    renameDialogOpen = false;
                    std::memset(renameBuffer, 0, 1024);
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
            ImGui::OpenPopup("Confirm Deletion");
        }
        if (ImGui::BeginPopupModal("Confirm Deletion", &deleteDialogOpen))
        {
            ImGui::Text("Are you sure you want to delete %s?",
                        selectedEntry.filename().string().c_str());
            if (ImGui::Button("Yes"))
            {
                if (deleteFile(selectedEntry))
                {
                    selectedEntry.clear();
                    deleteDialogOpen = false;
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                deleteDialogOpen = false;
            }
            ImGui::EndPopup();
        }
    }

    ImGui::End();
}

void FileExplorer::openFileWithDefaultEditor(const fs::path &filePath)
{
    std::string command;

#ifdef _WIN32
    command = "start \"\" \"" + filePath.string() + "\"";
#elif __APPLE__
    command = "open \"" + filePath.string() + "\"";
#else
    command = "xdg-open \"" + filePath.string() + "\"";
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
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error renaming file: " << e.what() << std::endl;
        return false;
    }
}

bool FileExplorer::deleteFile(const fs::path &filePath)
{
    try
    {
        fs::remove(filePath);
        return true;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error deleting file: " << e.what() << std::endl;
        return false;
    }
}

void render(FileExplorer &file_explorer)
{
    file_explorer.Draw("File Explorer Tool");
}
