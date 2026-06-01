/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class Character;
class Player;

namespace Network::Packets::Server { struct CharacterListPacket; }

struct Network::Packets::Server::CharacterListPacket final : public Network::Packet
{
    explicit CharacterListPacket(Player const &, std::vector<Ref<Character>> const & characters);
};
