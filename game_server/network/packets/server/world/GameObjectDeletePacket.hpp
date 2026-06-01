/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

class GameObject;

namespace Network::Packets::Server { struct GameObjectDeletePacket; }

struct Network::Packets::Server::GameObjectDeletePacket final : public Network::Packet
{
    explicit GameObjectDeletePacket(GameObject const &);
};
