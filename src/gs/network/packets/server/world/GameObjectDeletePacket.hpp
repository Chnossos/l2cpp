/// @author    Chnossos
/// @date      Created on 2026-04-05

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/Typedefs.hpp>

class GameObject;

namespace Network::Packets::Server { struct GameObjectDeletePacket; }

struct Network::Packets::Server::GameObjectDeletePacket final : public Network::Packet
{
    explicit GameObjectDeletePacket(GameObject const &);
    explicit GameObjectDeletePacket(GameObjectId);
};
