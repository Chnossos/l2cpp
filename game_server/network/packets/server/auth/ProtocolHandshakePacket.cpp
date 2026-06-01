/// @author    Chnossos
/// @date      Created on 2026-03-03

#include "ProtocolHandshakePacket.hpp"

using namespace Network::Packets::Server;

ProtocolHandshakeOkPacket::ProtocolHandshakeOkPacket(std::span<byte const> const encryptionKey)
{
    *this << encryptionKey;
}
