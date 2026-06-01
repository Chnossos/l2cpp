/// @author    Chnossos
/// @date      Created on 2026-03-23

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server
{
    /// Sun sets at 1am IG.
    using SunSetPacket = Network::HeaderOnlyPacket<0x1d>;
}
