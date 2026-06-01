/// @author    Chnossos
/// @date      Created on 2026-03-17

#pragma once

// Project includes
#include <common/network/Packet.hpp>

class Character;
class Monster;

namespace Network::Packets::Server { struct TargetMonsterSelectPacket; }

struct Network::Packets::Server::TargetMonsterSelectPacket final : public Network::Packet
{
    explicit TargetMonsterSelectPacket(Character const & source, Monster const & target);
};
