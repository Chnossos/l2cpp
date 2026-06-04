/// @author    Chnossos
/// @date      Created on 2026-05-04

#pragma once

// Project includes
#include <gs/game/ecs/ActorSystem.hpp>

namespace Network::Packets::Server { class StatsUpdatePacket; }

class Character;
class CharacterStatus;
class Npc;

struct ActorStatsUpdateSystem : public ActorSystem
{
    void updateImpl(ClockDuration elapsed, Actor &) override;

private:
    bool updateCharacterStats(Network::Packets::Server::StatsUpdatePacket & privatePacket,
                              Network::Packets::Server::StatsUpdatePacket & publicPacket,
                              Character &, Stats const & oldStats) const;

    void updateCharacterStatus(Network::Packets::Server::StatsUpdatePacket & privatePacket,
                               Network::Packets::Server::StatsUpdatePacket & publicPacket,
                               Character &, CharacterStatus const & oldStatus) const;

    void updateNpcStats(Npc &, Stats const & oldStats) const;
};
