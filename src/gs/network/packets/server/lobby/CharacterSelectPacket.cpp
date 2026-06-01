/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "CharacterSelectPacket.hpp"

// Project includes
#include "../../../../Player.hpp"
#include "../../../../game/World.hpp"
#include "../../../../game/actor/Character.hpp"
#include "../../../../game/components/CharacterStatus.hpp"
#include "../../../../game/components/PlayerAppearance.hpp"
#include "../../../../game/components/Position.hpp"
#include "../../../../game/components/Stats.hpp"

using Network::Packets::Server::CharacterSelectPacket;

CharacterSelectPacket::CharacterSelectPacket(Character const & c, u32 const encryptionKey)
    : Packet(0x15, "CharacterSelect")
{
    auto const & stats  = c.stats();
    auto const & status = c.status();

    *this
        << c.name()
        << c.id()
        << c.title()
        << encryptionKey
        << 0 // clanId
        << 0 // ?
        << c.appearance().sex()
        << c.appearance().race()
        << c.appearance().startingProfession()
        << 1 // active (?)
        << c.position()
        << stats[StatId::CurHp]
        << stats[StatId::CurMp]
        << status.sp()
        << status.xp()
        << status.level()
        << status.karma()
        << status.pkCount()
        << static_cast<u32>(stats[StatId::Int])
        << static_cast<u32>(stats[StatId::Str])
        << static_cast<u32>(stats[StatId::Con])
        << static_cast<u32>(stats[StatId::Men])
        << static_cast<u32>(stats[StatId::Dex])
        << static_cast<u32>(stats[StatId::Wit])
        << std::array<u32, 32>{} // ?
        << World::inGameTime()
        << 0
        << c.profession()
        << std::array<u32, 12>{} // ?
    ;
}
