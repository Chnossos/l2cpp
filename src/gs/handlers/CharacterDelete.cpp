/// @author    Chnossos
/// @date      Created on 2026-06-01

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/lobby/CharacterDeletePacket.hpp>

DECLARE_PACKET_HANDLER(CharacterList);

DEFINE_PACKET_HANDLER(CharacterDelete)
try
{
    L2CPP_B_ASSERT(!player.currentCharacter(), "Cannot delete a character while another one is in the world");

    PacketReader reader{player.connection().readBuffer().subspan(3)};
    u32 index;
    reader >> index;

    auto const characterPreviews = World::getCharacterPreviews(player);
    L2CPP_B_ASSERT(!characterPreviews.empty(),
                   "Cannot delete a character from an account that has none");
    L2CPP_B_ASSERT(index < characterPreviews.size(),
                   "Invalid deletion index '{}': should be less than {}", index, characterPreviews.size());

    World::delCharacterPreview(player.accountId(), index);
    player.connection().send(CharacterDeletePacket{});
    handleCharacterList(player);
}
catch (Core::Exception const & e)
{
    SPDLOG_ERROR("Failed to delete character:{}\n", Core::formatExceptionStack(e));
    player.connection().send(CharacterDeletePacket{CharacterDeletionFailureReason::GeneralFailure});
    throw;
}
