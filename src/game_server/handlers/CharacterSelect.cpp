/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "../game/World.hpp"
#include "../game/actor/Character.hpp"
#include "../game/components/CharacterSelectionData.hpp"
#include "../network/packets/server/client/ClientForceDisconnectPacket.hpp"
#include "../network/packets/server/lobby/CharacterSelectPacket.hpp"
#include "../orm/Lobby.hpp"
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(CharacterSelect) try
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 index;
    reader >> index;

    auto const characterPreviews = World::getCharacterPreviews(player);
    L2CPP_B_ASSERT(index < characterPreviews.size(),
                   "Invalid character selection index '{}': should be less than {}", index, characterPreviews.size());

    Orm::selectCharacter(player.accountId(), characterPreviews[index].get().name());

    for (Character & c : characterPreviews)
        c.component<CharacterSelectionData>()->selected = false;
    characterPreviews[index].get().component<CharacterSelectionData>()->selected = true;

    player.setCurrentCharacter(World::loadCharacterFromPreview(characterPreviews[index]));
    player.connection().send(CharacterSelectPacket(player.currentCharacter(), player.playOk1()));
}
catch (...)
{
    // Any exception would get the client stuck during the login screen; force disconnect him instead.
    player.connection().send(ClientForceDisconnectPacket());
    player.connection().close();
    throw;
}
