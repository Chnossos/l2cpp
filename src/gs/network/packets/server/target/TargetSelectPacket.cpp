/// @author    Chnossos
/// @date      Created on 2026-03-17

#include "TargetSelectPacket.hpp"

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/Position.hpp>

using Network::Packets::Server::TargetSelectPacket;

TargetSelectPacket::TargetSelectPacket(Character const & source, Actor const & target)
    : Packet(0x29, "TargetSelect")
{
    *this
        << source.id()
        << target.id()
        << source.position()
    ;
}
