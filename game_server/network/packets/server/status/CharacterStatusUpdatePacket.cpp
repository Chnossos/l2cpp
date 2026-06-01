/// @author    Chnossos
/// @date      Created on 2026-02-28

#include "CharacterStatusUpdatePacket.hpp"

// Project includes
#include "../../../../game/actor/Character.hpp"
#include "../../../../game/components/CharacterStatus.hpp"
#include "../../../../game/components/Gear.hpp"
#include "../../../../game/components/PlayerAppearance.hpp"
#include "../../../../game/components/Position.hpp"
#include "../../../../game/components/Stats.hpp"
#include "../../../../game/inventory/ItemStorage.hpp"

using namespace Network::Packets::Server;

CharacterStatusUpdatePacket::CharacterStatusUpdatePacket(Character const & c)
    : Packet(0x04, "CharacterStatusUpdate")
{
    using enum GearSlot;

    auto const & stats = c.stats();
    auto const weapon  = c.gear().weapon();

    *this
        << c.position()
        << c.position().orientation
        << c.id()
        << c.name()
        << c.appearance().race()
        << c.appearance().sex()
        << c.appearance().startingProfession()
        << c.status().level()
        << c.status().xp()
        << static_cast<u32>(stats[StatId::Str])
        << static_cast<u32>(stats[StatId::Dex])
        << static_cast<u32>(stats[StatId::Con])
        << static_cast<u32>(stats[StatId::Int])
        << static_cast<u32>(stats[StatId::Wit])
        << static_cast<u32>(stats[StatId::Men])
        << static_cast<u32>(stats[StatId::MaxHp])
        << static_cast<u32>(stats[StatId::CurHp])
        << static_cast<u32>(stats[StatId::MaxMp])
        << static_cast<u32>(stats[StatId::CurMp])
        << c.status().sp()
        << static_cast<u32>(stats[StatId::CurWeight])
        << static_cast<u32>(stats[StatId::MaxWeight])
        << (c.gear().hasActiveWeapon() ? 20 : 40)
        << c.gear().itemId(Underwear)
        << c.gear().itemId(RightEar)
        << c.gear().itemId(LeftEar)
        << c.gear().itemId(Neck)
        << c.gear().itemId(RightFinger)
        << c.gear().itemId(LeftFinger)
        << c.gear().itemId(Head)
        << c.gear().itemId(RightHand)
        << (weapon && weapon->tmplate.gearSlot == Hands ? 0 : c.gear().itemId(LeftHand))
        << c.gear().itemId(Gloves)
        << c.gear().itemId(Chest)
        << c.gear().itemId(Legs)
        << c.gear().itemId(Feet)
        << c.gear().itemId(Back)
        << c.gear().itemId(RightHand)
        << c.gear().itemId(Hair)
        << c.gear().itemTemplateId(Underwear)
        << c.gear().itemTemplateId(RightEar)
        << c.gear().itemTemplateId(LeftEar)
        << c.gear().itemTemplateId(Neck)
        << c.gear().itemTemplateId(LeftFinger)
        << c.gear().itemTemplateId(RightFinger)
        << c.gear().itemTemplateId(Head)
        << c.gear().itemTemplateId(RightHand)
        << (weapon && weapon->tmplate.gearSlot == Hands ? 0 : c.gear().itemTemplateId(LeftHand))
        << c.gear().itemTemplateId(Gloves)
        << c.gear().itemTemplateId(Chest)
        << c.gear().itemTemplateId(Legs)
        << c.gear().itemTemplateId(Feet)
        << c.gear().itemTemplateId(Back)
        << c.gear().itemTemplateId(RightHand)
        << c.gear().itemTemplateId(Hair)
        << static_cast<u32>(stats[StatId::PAtk])
        << static_cast<u32>(stats[StatId::PAtkSpeed])
        << static_cast<u32>(stats[StatId::PDef])
        << static_cast<u32>(stats[StatId::Evasion])
        << static_cast<u32>(stats[StatId::Accuracy])
        << static_cast<u32>(stats[StatId::PCritRate])
        << static_cast<u32>(stats[StatId::MAtk])
        << static_cast<u32>(stats[StatId::MAtkSpeed])
        << static_cast<u32>(stats[StatId::BasePAtkSpeed])
        << static_cast<u32>(stats[StatId::MDef])
        << 0 // (c.isPvpFlagged ? 1 : 0)
        << c.status().karma()
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
        << c.appearance().collisionRadius
        << c.appearance().collisionHeight
        << c.appearance().hairStyle()
        << c.appearance().hairColor()
        << c.appearance().face()
        << (c.accessLevel > 0 ? 1 : 0)
        << c.title()
        << 0                  // clanId
        << 0                  // crest id
        << 0                  // ally id
        << 0                  // ally crest id
        << 0                  // clan leader: 0x60 | in-siege: 0x40 | leader rights: 0x20
        << static_cast<u8>(0) // mount type
        << static_cast<u8>(0) // private store type
        << false              // can dwarven craft
        << c.status().pkCount()
        << c.status().pvpCount()
    ;

    std::array<u16, 0> const cubics;
    *this << static_cast<u16>(cubics.size());
    for (auto const id : cubics)
        *this << id;

    *this
        << false // looking for party members
        << 0     // abnormal effects
        << false // is inside water
        << 0     // clan privileges
        << 0     // 0x100 - swim?
        << 0     // ?
        << 0     // ?
        << 0     // ?
        << 0     // ?
        << 0     // ?
        << 0     // ?
        << 0_u16 // evalAmount
        << 0_u16 // evalScore
        << 0     // ?
        << c.inventory().limit()
        << c.profession()
        << 0 // special effects? circles around player...
        << static_cast<u32>(stats[StatId::MaxCp])
        << static_cast<u32>(stats[StatId::CurCp])
        << (weapon ? weapon->enchantLevel : 0_u8)
        << c.team()
        << 0 // clan crest large id
        << c.status().isNoblesse() // noblesse symbol in status window
        << c.status().isHero()     // hero aura
        << false // is fishing
        << 0 // fish x
        << 0 // fish y
        << 0 // fish z
        << c.appearance().nameColor()
    ;
}
