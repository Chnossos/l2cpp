/// @author    Chnossos
/// @date      Created on 2026-02-27

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/skill/SkillListPacket.hpp>

DEFINE_PACKET_HANDLER(SkillList)
{
    L2CPP_B_ASSERT(player.currentCharacter(), "No character, can't send skill list");
    player.connection().send(SkillListPacket(player.currentCharacter()->skills()));
}
