/// @author    Chnossos
/// @date      Created on 2026-04-21

#include "ClientForceDisconnectPacket.hpp"

using Network::Packets::Server::ClientForceDisconnectPacket;

ClientForceDisconnectPacket::ClientForceDisconnectPacket()
    : Packet(0x26, "ClientForceDisconnect")
{}
