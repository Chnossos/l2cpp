/// @author    Chnossos
/// @date      Created on 2026-04-26

#pragma once

// Project includes
#include <common/network/Packet.hpp>
#include <gs/network/packets/server/i18n/SystemMessageArgument.hpp>

namespace Network::Packets::Server { class UiConfirmationModalShowPacket; }

class Network::Packets::Server::UiConfirmationModalShowPacket final : public Packet
{
public:
    explicit UiConfirmationModalShowPacket(u32 systemMessageId);

public:
    template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
    UiConfirmationModalShowPacket & appendArg(T && arg) { return appendArgImpl(std::forward<T>(arg)); }

private:
    UiConfirmationModalShowPacket & appendArgImpl(SystemMessageArgument const & arg);

private:
    u32 _argsCountOffset;
};

template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
Network::Packets::Server::UiConfirmationModalShowPacket &
operator<<(Network::Packets::Server::UiConfirmationModalShowPacket & p, T && arg) {
    return p.appendArg(std::forward<T>(arg));
}
