/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "_Common.hpp"
#include "../game/World.hpp"
#include "../network/packets/server/client/ClientGoBackToCharacterSelectionPacket.hpp"

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
