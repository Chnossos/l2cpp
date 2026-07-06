/// @author    Chnossos
/// @date      Created on 2026-04-03

#include "ActorDiePacket.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>

using Network::Packets::Server::ActorDiePacket;

namespace
{
    void writePacket(Network::Packet & p, Actor const & actor,
                     bool const canMoveToVillage, bool const canMoveToClanHall, bool const canMoveToCastle,
                     bool const canMoveToSiegeHq, bool const isSweepable,       bool const canReviveOnTheSpot)
    {
        p
            << actor.id()
            << static_cast<u32>(canMoveToVillage)
            << static_cast<u32>(canMoveToClanHall)
            << static_cast<u32>(canMoveToCastle)
            << static_cast<u32>(canMoveToSiegeHq)
            << static_cast<u32>(isSweepable)
            << static_cast<u32>(canReviveOnTheSpot)
        ;
    }
}

ActorDiePacket::ActorDiePacket(Actor const & actor)
    : Packet(0x06, "ActorDie")
{
    if (actor.type() == ActorType::Character)
        ::writePacket(*this, actor, true, true, true, true, false, true);
    else
        ::writePacket(*this, actor, false, false, false, false, false, false);
}
