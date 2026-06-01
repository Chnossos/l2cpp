/// @author    Chnossos
/// @date      Created on 2026-03-03

#pragma once

// Project includes
#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server
{
    namespace details
    {
        template<bool ok>
        struct ProtocolHandshakeBase : public Network::Packet
        {
            ProtocolHandshakeBase(): Packet(0x00, ok ? "ProtocolHandshakeOk" : "ProtocolHandshakeFail") { *this << ok; }
        };
    }

    using ProtocolHandshakeFailPacket = details::ProtocolHandshakeBase<false>;

    struct ProtocolHandshakeOkPacket final : public details::ProtocolHandshakeBase<true>
    {
        explicit ProtocolHandshakeOkPacket(std::span<byte const> encryptionKey);
    };
}
