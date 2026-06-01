/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/handlers/_Common.hpp>

DECLARE_PACKET_HANDLER(CharacterList)

DEFINE_PACKET_HANDLER(CharacterCancelDeletion)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 characterId;
    reader >> characterId;

    if (auto const c = World::character(characterId); c)
        /*c->deleteTime = 0*/;

    handleCharacterList(player);
}
