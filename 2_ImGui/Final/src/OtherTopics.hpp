#pragma once

#include <string_view>

#include "BaseView.hpp"

class OtherTopics : public BaseView
{
public:
    OtherTopics(){};
    virtual ~OtherTopics(){};

    void Draw(std::string_view label, bool *open = nullptr) override;
};
