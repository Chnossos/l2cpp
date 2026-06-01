/// @author    Chnossos
/// @date      Created on 2026-03-21

#include "AttackStanceTogglePacket.hpp"

// Project includes
#include "../../../../game/actor/Actor.hpp"

using Network::Packets::Server::AttackStanceTogglePacket;

AttackStanceTogglePacket::AttackStanceTogglePacket(bool const enable, Actor const & target)
    : Packet(enable ? 0x2b : 0x2c, enable ? "AttackStanceEnable" : "AttackStanceDisable")
{
    *this
        << target.id()
    ;
}
