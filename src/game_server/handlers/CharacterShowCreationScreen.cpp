/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(CharacterShowCreationScreen)
{
    player.connection().send(Packet(0x17, "CharacterShowCreationScreen") << 0);
}
