/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "../game/World.hpp"
#include "../network/packets/server/client/ClientExitGamePacket.hpp"
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(ConnectionClosing)
{
    if (auto const c = player.currentCharacter()) // note: client doesn't wait for an answer during character selection
    {
        World::moveCharacterBackToPreviews(c);
        player.connection().send(ClientExitGamePacket());
    }

    player.connection().close();
}
