/// @author    Chnossos
/// @date      Created on 2026-04-21

#include "ClientForceDisconnectPacket.hpp"

using namespace Network::Packets::Server;

ClientForceDisconnectPacket::ClientForceDisconnectPacket()
    : Packet(0x26, "ClientForceDisconnect")
{}
