/// @author    Chnossos
/// @date      Created on 2026-03-23

#pragma once

// Project includes
#include <common/network/Packet.hpp>

namespace Network::Packets::Server
{
    /// Sun rises at 7am IG.
    using SunRisePacket = Network::HeaderOnlyPacket<0x1c>;
}
