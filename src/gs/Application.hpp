/// @author    Chnossos
/// @date      Created on 2026-03-06

#pragma once

// Project includes
#include <common/Pimpl.hpp>

// C++ includes
#include <string_view>
#include <vector>

class Application
{
public:
    explicit Application(std::vector<std::string_view> args);
    ~Application();

public:
    bool load();
    int  run();

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
