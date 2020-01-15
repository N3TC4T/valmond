#pragma once

#include <json/json.h>

#include "common/config.hpp"

class Agent
{
public:
    explicit Agent(Config* cfg);
    int
    beat();

private:
    [[nodiscard]] Json::Value
    collectData();
    void
    sendData(std::string data);
    Config* config;
};
