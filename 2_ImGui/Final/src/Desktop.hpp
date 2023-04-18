#pragma once

#include <random>
#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include <fmt/format.h>

#include "AdvCalc.hpp"
#include "Calender.hpp"
#include "Clock.hpp"
#include "CsvEditor.hpp"
#include "Diff.hpp"
#include "FileExplorer.hpp"
#include "MediaPlayer.hpp"
#include "OtherTopics.hpp"
#include "Paint.hpp"
#include "TextEditor.hpp"
#include "WindowBase.hpp"

class Desktop : public WindowBase
{
public:
    struct Icon
    {
        Icon(std::string_view label_, WindowBase *base_)
            : label(label_), base(base_), position(0.0F, 0.0F){};
        void Draw();

        int clicked_count = 0;
        bool popup_open = false;
        ImVec2 position;
        std::string label;
        WindowBase *base = nullptr;
    };

public:
    Desktop()
        : adv_calc(), calender(), diff_viewer(), file_explorer(),
          media_player(), paint(), text_editor(), csv_editor(), clock({}),
          icons({})
    {
        icons.reserve(7);
        icons.push_back(Icon{"AdvCalc", &adv_calc});
        icons.push_back(Icon{"Calender", &calender});
        icons.push_back(Icon{"DiffViewer", &diff_viewer});
        icons.push_back(Icon{"FileExplorer", &file_explorer});
        icons.push_back(Icon{"MediaPlayer", &media_player});
        icons.push_back(Icon{"Paint", &paint});
        icons.push_back(Icon{"TextEditor", &text_editor});
        icons.push_back(Icon{"CsvEditor", &csv_editor});
        icons.push_back(Icon{"OtherTopics", &other_topics});

        LoadTheme();
    };
    virtual ~Desktop(){};

    void Draw(std::string_view label, bool *open = nullptr) override;
    void ShowIconList(bool *open);

private:
    AdvCalc adv_calc;
    Calender calender;
    DiffViewer diff_viewer;
    FileExplorer file_explorer;
    MediaPlayer media_player;
    Paint paint;
    TextEditor text_editor;
    CsvEditor csv_editor;
    OtherTopics other_topics;

    Clock clock;

    std::vector<Icon> icons;
};

void render(Desktop &window_class);
