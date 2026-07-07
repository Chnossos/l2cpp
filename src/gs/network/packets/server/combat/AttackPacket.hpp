/// @author    Chnossos
/// @date      Created on 2026-03-21

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/game/combat/Hit.hpp>

class Actor;

namespace Network::Packets::Server { class AttackPacket; }

class Network::Packets::Server::AttackPacket final : public Packet
{
public:
    explicit AttackPacket(Actor const & attacker, Actor const & mainTarget);

public:
    void addHit(Hit const & hit);

private:
    Actor const & _attacker;
    Actor const & _mainTarget;
    u16           _hitsCountOffset;
};
