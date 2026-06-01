/// @author    Chnossos
/// @date      Created on 2026-04-05

#include "TargetClearPacket.hpp"

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/Position.hpp>

using Network::Packets::Server::TargetClearPacket;

TargetClearPacket::TargetClearPacket(Character const & c)
    : Packet(0x2a, "TargetClear")
{
    *this
        << c.id()
        << c.position()
    ;
}
