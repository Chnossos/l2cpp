/// @author    Chnossos
/// @date      Created on 2026-04-21

#include "ClientExitGamePacket.hpp"

using Network::Packets::Server::ClientExitGamePacket;

ClientExitGamePacket::ClientExitGamePacket()
    : Packet(0x7e, "ClientExitGame")
{}
