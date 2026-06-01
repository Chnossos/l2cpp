/// @author    Chnossos
/// @date      Created on 2026-04-08

#include "SkillUsePacket.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>
#include <gs/game/components/Position.hpp>

using Network::Packets::Server::SkillUsePacket;

SkillUsePacket::SkillUsePacket(
    Actor         const & caster
  , Actor         const & target
  , SkillUid      const   skillUid
  , ClockDuration const   castDuration
  , ClockDuration const   cooldown
  , bool          const   isCritical
)
    : Packet(0x48, "SkillUse")
{
    *this
        << caster.id()
        << target.id()
        << skillUid
        << std::chrono::floor<std::chrono::milliseconds>(castDuration)
        << std::chrono::floor<std::chrono::milliseconds>(cooldown)
        << caster.position()
        << (isCritical ? 1 : 0)
    ;
}
