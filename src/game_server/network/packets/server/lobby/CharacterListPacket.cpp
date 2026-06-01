/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "CharacterListPacket.hpp"

// Project includes
#include "../../../../Player.hpp"
#include "../../../../game/actor/Character.hpp"
#include "../../../../game/components/CharacterSelectionData.hpp"
#include "../../../../game/components/CharacterStatus.hpp"
#include "../../../../game/components/Gear.hpp"
#include "../../../../game/components/PlayerAppearance.hpp"
#include "../../../../game/components/Position.hpp"
#include "../../../../game/components/Stats.hpp"

using Network::Packets::Server::CharacterListPacket;

CharacterListPacket::CharacterListPacket(Player const & player, std::vector<Ref<Character>> const & characters)
    : Packet(0x13, "CharacterList")
{
    *this << static_cast<u32>(characters.size());

    // ReSharper disable once CppRangeBasedForIncompatibleReference
    for (Character const & c : characters)
    {
        using enum GearSlot;

        auto const weapon = c.gear().weapon();

        *this
            << c.name()
            << c.id()
            << player.accountName()
            << player.playOk1()
            << 0 // clanId
            << 0 // ?
            << c.appearance().sex()
            << c.appearance().race()
            << c.appearance().startingProfession()
            << 1 // active (?)
            << c.position()
            << c.stats()[StatId::CurHp]
            << c.stats()[StatId::CurMp]
            << c.status().sp()
            << c.status().xp()
            << c.status().level()
            << c.status().karma()
            << std::array<u32, 9>{} // unknown
            << c.gear().itemId(Underwear)
            << c.gear().itemId(LeftEar)
            << c.gear().itemId(RightEar)
            << c.gear().itemId(Neck)
            << c.gear().itemId(LeftFinger)
            << c.gear().itemId(RightFinger)
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
            << c.gear().itemTemplateId(LeftEar)
            << c.gear().itemTemplateId(RightEar)
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
            << c.appearance().hairStyle()
            << c.appearance().hairColor()
            << c.appearance().face()
            << c.stats()[StatId::MaxHp]
            << c.stats()[StatId::MaxMp]
            << 0 // remainingSecondsBeforeDeletion
            << c.profession()
            << (c.component<CharacterSelectionData>()->selected ? 1 : 0)
            << (weapon ? weapon->enchantLevel : 0_u8)
        ;
    }
}
