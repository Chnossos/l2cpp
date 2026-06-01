/// @author    Chnossos
/// @date      Created on 2026-03-02

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../constants/ShortcutType.hpp"

#include <common/network/Serialization.hpp>

class Shortcut
{
    DECLARE_PACKET_SERIALIZATION_OPERATOR(Shortcut);

public:
    using Index = u32;

public:
    virtual ~Shortcut();

protected:
    Shortcut(Index index, ShortcutType type);

public:
            auto index()     const -> Index;
            auto type()      const -> ShortcutType;
    virtual auto targetId()  const -> u32 = 0;

private:
    void serialize(Network::Packet &) const;
    virtual void serializeImpl(Network::Packet &) const = 0;

private:
    Index        _index;
    ShortcutType _type;
};
