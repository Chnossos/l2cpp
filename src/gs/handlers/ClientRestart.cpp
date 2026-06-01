/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <gs/game/World.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/client/ClientGoBackToCharacterSelectionPacket.hpp>

DECLARE_PACKET_HANDLER(CharacterList)

DEFINE_PACKET_HANDLER(ClientRestart)
{
    if (auto const c = player.currentCharacter())
    {
        World::moveCharacterBackToPreviews(c);
        player.connection().send(ClientGoBackToCharacterSelectionPacket(true));
    }

    handleCharacterList(player);
}
