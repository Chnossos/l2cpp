/// @author    Chnossos
/// @date      Created on 2026-02-28

// Project includes
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(MiniMapToggle)
{
    player.connection().send(Packet(0x9d, "MiniMapToggle") << 1665);
}
