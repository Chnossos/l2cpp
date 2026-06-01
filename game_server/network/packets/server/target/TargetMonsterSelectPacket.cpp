/// @author    Chnossos
/// @date      Created on 2026-03-17

#include "TargetMonsterSelectPacket.hpp"

// Project includes
#include "../../../../game/actor/Character.hpp"
#include "../../../../game/actor/Monster.hpp"
#include "../../../../game/components/CharacterStatus.hpp"
#include "../../../../game/components/NpcStatus.hpp"

using Network::Packets::Server::TargetMonsterSelectPacket;

TargetMonsterSelectPacket::TargetMonsterSelectPacket(Character const & source, Monster const & target)
    : Packet(0xa6, "TargetMonsterSelect")
{
    *this
        << target.id()
        << static_cast<u16>(source.status().level() - target.status().level())
    ;
}
