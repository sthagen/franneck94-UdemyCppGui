#pragma once

#include <string_view>

#include "WindowBase.hpp"

class OtherTopics : public WindowBase
{
public:
    OtherTopics() {};
    virtual ~OtherTopics(){};

    void Draw(std::string_view label, bool *open = nullptr) override;
};
