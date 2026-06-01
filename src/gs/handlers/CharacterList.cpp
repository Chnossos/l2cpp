/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <gs/game/World.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/lobby/CharacterListPacket.hpp>

DEFINE_PACKET_HANDLER(CharacterList)
{
    player.connection().send(CharacterListPacket(player, World::getCharacterPreviews(player)));
}
