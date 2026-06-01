/// @author    Chnossos
/// @date      Created on 2026-04-26

#pragma once

// Project includes
#include "../i18n/SystemMessageArgument.hpp"

#include <l2cpp/network/Packet.hpp>

namespace Network::Packets::Server { class UiConfirmationModalShowPacket; }

class Network::Packets::Server::UiConfirmationModalShowPacket final : public Network::Packet
{
public:
    explicit UiConfirmationModalShowPacket(u32 messageId);

public:
    template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
    UiConfirmationModalShowPacket & appendArg(T && arg) { return appendArgImpl(arg); }

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
