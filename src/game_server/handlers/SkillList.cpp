/// @author    Chnossos
/// @date      Created on 2026-02-27

// Project includes
#include "_Common.hpp"
#include "../game/actor/Character.hpp"
#include "../network/packets/server/skill/SkillListPacket.hpp"

DEFINE_PACKET_HANDLER(SkillList)
{
    L2CPP_B_ASSERT(player.currentCharacter(), "No character, can't send skill list");
    player.connection().send(SkillListPacket(player.currentCharacter()->skills()));
}
