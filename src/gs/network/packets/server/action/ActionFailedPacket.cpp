/// @author    Chnossos
/// @date      Created on 2026-04-21

#include "ActionFailedPacket.hpp"

Network::Packets::Server::ActionFailedPacket::ActionFailedPacket()
    : Packet(0x25, "ActionFailed")
{}
