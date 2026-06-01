/// @author    Chnossos
/// @date      Created on 2026-03-16

#include "NpcStatusUpdatePacket.hpp"

// Project includes
#include <gs/game/actor/Npc.hpp>
#include <gs/game/components/Gear.hpp>
#include <gs/game/components/NpcAppearance.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/components/Stats.hpp>

using Network::Packets::Server::NpcStatusUpdatePacket;

NpcStatusUpdatePacket::NpcStatusUpdatePacket(Npc const & actor)
    : Packet(0x16, "NpcStatusUpdate")
{
    auto const & appearance = actor.appearance();
    auto const & gear       = actor.gear();
    auto const & stats      = actor.stats();

    *this
        << actor.id()
        << appearance.id()
        << (actor.isAttackable() ? 1 : 0)
        << actor.position()
        << actor.position().orientation
        << 0 // ?
        << static_cast<u32>(stats[StatId::MAtkSpeed])
        << static_cast<u32>(stats[StatId::PAtkSpeed])
        << static_cast<u32>(stats[StatId::RunSpeed])
        << static_cast<u32>(stats[StatId::WalkSpeed])
        << static_cast<u32>(stats[StatId::SwimRunSpeed])
        << static_cast<u32>(stats[StatId::SwimWalkSpeed])
        << static_cast<u32>(stats[StatId::FlyRunSpeed])
        << static_cast<u32>(stats[StatId::FlyWalkSpeed])
        << static_cast<u32>(stats[StatId::FlyRunSpeed])
        << static_cast<u32>(stats[StatId::FlyWalkSpeed])
        << stats[StatId::RunSpeed ] / stats[StatId::BaseRunSpeed ]
        << stats[StatId::PAtkSpeed] / stats[StatId::BasePAtkSpeed]
        << appearance.collisionRadius
        << appearance.collisionHeight
        << gear.itemTemplateId(GearSlot::RightHand)
        << gear.itemTemplateId(GearSlot::Chest)
        << gear.itemTemplateId(GearSlot::LeftHand)
        << appearance.nameIsVisible()
        << true // isRunning
        << actor.isInCombatStance()
        << false // is like dead
        << 0_u8  // visibility: 0=visible 1=invisible 2=summoned (??)
        << actor.name()
        << actor.title()
        << appearance.titleColor()
        // next properties taken from a summon's owner
        << 0 // not pvp flagged
        << 0 // no karma
        << 0 // no visual effect
        << 0 // no clan
        << 0 // no clan crest
        << 0 // no ally
        << 0 // no ally crest
        << false // not in water nor flying
        << actor.team()
        << appearance.collisionRadius
        << appearance.collisionHeight
    ;

    auto const weapon = gear.weapon();
    *this << (weapon ? weapon->enchantLevel : 0);
}
