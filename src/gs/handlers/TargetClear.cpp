/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "_Common.hpp"
#include "../game/World.hpp"
#include "../game/actor/Character.hpp"
#include "../network/packets/server/target/TargetClearPacket.hpp"

DEFINE_PACKET_HANDLER(TargetClear)
{
    L2CPP_B_ASSERT(player.currentCharacter(), "Cannot cancel skill or clear target, no current character");

    auto & c = *player.currentCharacter();

    /**/ if (auto const action = c.currentAction(); action && action->type() == ActionType::Skill)
        c.currentAction()->cancel();
    else if (auto const target = c.target())
    {
        World::unsubscribeFromTarget(target, c);
        c.setTarget(std::nullopt);
        player.connection().send(TargetClearPacket{c});
    }
}
