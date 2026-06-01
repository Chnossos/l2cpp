/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include "ActorAppearance.hpp"

class NpcAppearance : public ActorAppearance
{
public:
    NpcAppearance() noexcept;

public:
    auto id() const -> u32;
    auto titleColor() const -> u32;
    bool nameIsVisible() const;

public:
    void setId(u32 id);
    void setTitleColor(u32 color);
    void setNameIsVisible(bool visible);

private:
    u32 _id, _titleColor;
    bool _nameIsVisible;
};
