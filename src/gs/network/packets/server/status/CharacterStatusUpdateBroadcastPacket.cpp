/// @author    Chnossos
/// @date      Created on 2026-02-28

#include "CharacterStatusUpdateBroadcastPacket.hpp"

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/Gear.hpp>
#include <gs/game/components/PlayerAppearance.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/components/Stats.hpp>

using Network::Packets::Server::CharacterStatusUpdateBroadcastPacket;

CharacterStatusUpdateBroadcastPacket::CharacterStatusUpdateBroadcastPacket(Character const & c)
    : Packet(0x03, "CharacterStatusUpdateBroadcast")
{
    using enum GearSlot;

    auto const & appearance = c.appearance();
    auto const & gear       = c.gear();
    auto const & stats      = c.stats();
    auto const weapon       = gear.weapon();

    *this
        << c.position()
        << 0 // vehicleId
        << c.id()
        << c.name()
        << appearance.race()
        << appearance.sex()
        << c.appearance().startingProfession()
        << 0 // ?
        << gear.itemTemplateId(Head)
        << gear.itemTemplateId(RightHand)
        << (weapon && weapon->tmplate.gearSlot == Hands ? 0 : gear.itemTemplateId(LeftHand))
        << gear.itemTemplateId(Gloves)
        << gear.itemTemplateId(Chest)
        << gear.itemTemplateId(Legs)
        << gear.itemTemplateId(Feet)
        << gear.itemTemplateId(Back)
        << gear.itemTemplateId(RightHand)
        << gear.itemTemplateId(Hair)
        << 0 // (c.isPvpFlagged ? 1 : 0)
        << c.status().karma()
        << static_cast<u32>(stats[StatId::MAtkSpeed])
        << static_cast<u32>(stats[StatId::PAtkSpeed])
        << 0 // (c.isPvpFlagged ? 1 : 0) // repeated
        << c.status().karma()            // repeated
        << static_cast<u32>(stats[StatId::BaseRunSpeed])
        << static_cast<u32>(stats[StatId::BaseWalkSpeed])
        << static_cast<u32>(stats[StatId::BaseSwimRunSpeed])
        << static_cast<u32>(stats[StatId::BaseSwimWalkSpeed])
        << static_cast<u32>(stats[StatId::BaseFlyRunSpeed])
        << static_cast<u32>(stats[StatId::BaseFlyWalkSpeed])
        << static_cast<u32>(stats[StatId::BaseFlyRunSpeed])
        << static_cast<u32>(stats[StatId::BaseFlyWalkSpeed])
        << stats[StatId::RunSpeed ] / stats[StatId::BaseRunSpeed ]
        << stats[StatId::PAtkSpeed] / stats[StatId::BasePAtkSpeed]
        << appearance.collisionRadius()
        << appearance.collisionHeight()
        << appearance.hairStyle()
        << appearance.hairColor()
        << appearance.face()
        << c.title()
        << 0     // clanId
        << 0     // clan crest id
        << 0     // alliance id
        << 0     // alliance crest id
        << 0     // ?
        << true  // standing
        << true  // running
        << c.isInCombatStance()
        << c.isDead()
        << false // not invisible
        << 0_u8  // mount type (0=none 1=Strider 2=Wyvern)
        << 0_u8  // private store type
    ;

    std::array<u16, 0> const cubics;
    *this << static_cast<u16>(cubics.size());
    for (auto const id : cubics)
        *this << id;

    *this
        << false // looking for party members
        << 0     // abnormal visual effects
        << 0_u8  // evalAmount
        << 0_u16 // evalScore
        << c.profession()
        << static_cast<u32>(stats[StatId::MaxCp])
        << static_cast<u32>(stats[StatId::CurCp])
        << (weapon ? weapon->enchantLevel : 0_u8)
        << c.team()
        << 0 // clan large crest id
        << c.status().isNoblesse()
        << c.status().isHero()
        << false // fishing
        << 0 // fish x
        << 0 // fish y
        << 0 // fish z
        << appearance.nameColor()
    ;
}
