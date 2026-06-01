/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "_Common.hpp"
#include "../game/World.hpp"
#include "../network/packets/server/lobby/CharacterListPacket.hpp"

DEFINE_PACKET_HANDLER(CharacterList)
{
    player.connection().send(CharacterListPacket(player, World::getCharacterPreviews(player)));
}
