/// @author    Chnossos
/// @date      Created on 2026-03-21

#include "AttackPacket.hpp"

// Project includes
#include <gs/game/actor/Actor.hpp>
#include <gs/game/components/Gear.hpp>
#include <gs/game/components/Position.hpp>

using Network::Packets::Server::AttackPacket;

static constexpr size_t gPositionSize = sizeof(Position::x) + sizeof(Position::y) + sizeof(Position::z);

AttackPacket::AttackPacket(Actor const & attacker, Actor const & mainTarget)
    : Packet(0x05, "Attack")
    , _attacker(attacker)
    , _mainTarget(mainTarget)
    , _hitsCountOffset(0)
{
    *this << attacker.id();
}

void AttackPacket::addHit(Hit const & hit)
{
    if (!_hitsCountOffset) // first hit to be recorded
    {
        *this
            << hit
            << _attacker.position()
        ;

        appendCounterAndStoreOffset(_hitsCountOffset);
    }
    else
    {
        auto & counter = counterAtOffset(_hitsCountOffset);
        if (counter)
            erase(gPositionSize);

        *this << hit;
        ++counter;
    }

    *this << _mainTarget.position();
}
