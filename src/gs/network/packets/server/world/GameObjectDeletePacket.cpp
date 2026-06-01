/// @author    Chnossos
/// @date      Created on 2026-04-05

#include "GameObjectDeletePacket.hpp"

// Project includes
#include <gs/game/GameObject.hpp>

using Network::Packets::Server::GameObjectDeletePacket;

GameObjectDeletePacket::GameObjectDeletePacket(GameObject const & object)
    : Packet(0x12, "GameObjectDelete")
{
    *this
        << object.id()
    ;
}
