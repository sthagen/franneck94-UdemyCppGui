#pragma once

#include <random>
#include <string>
#include <string_view>

#include "imgui.h"
#include "imgui_stdlib.h"
#include "implot.h"
#include "miniaudio.h"
#include <fmt/format.h>

#include "AdvCalc.hpp"
#include "BaseView.hpp"
#include "Calender.hpp"
#include "Diff.hpp"
#include "FileExplorer.hpp"
#include "MediaPlayer.hpp"
#include "Paint.hpp"
#include "TextEditor.hpp"

class Desktop : public BaseView
{
public:
    struct Icon
    {
        Icon(std::string_view label_, BaseView *base_)
            : label(label_), base(base_){};
        void Draw();

        int clicked_count = 0;
        bool popup_open = false;
        ImVec2 position;
        std::string label;
        BaseView *base = nullptr;
    };

public:
    Desktop()
        : adv_calc({}), calender({}), diff_viewer({}), file_explorer({}),
          media_player({}), paint({}), text_editor({})
    {
        icons.reserve(7);
        icons.push_back(Icon{"AdvCalc", &adv_calc});
        icons.push_back(Icon{"Calender", &calender});
        icons.push_back(Icon{"DiffViewer", &diff_viewer});
        icons.push_back(Icon{"FileExplorer", &file_explorer});
        icons.push_back(Icon{"MediaPlayer", &media_player});
        icons.push_back(Icon{"Paint", &paint});
        icons.push_back(Icon{"TextEditor", &text_editor});
    };
    virtual ~Desktop() = default;

    void Draw(std::string_view label, bool *open = NULL) override;
    void ShowIconList();

private:
    AdvCalc adv_calc;
    Calender calender;
    DiffViewer diff_viewer;
    FileExplorer file_explorer;
    MediaPlayer media_player;
    Paint paint;
    TextEditor text_editor;

    std::vector<Icon> icons;
};

void render(Desktop &window_class);
