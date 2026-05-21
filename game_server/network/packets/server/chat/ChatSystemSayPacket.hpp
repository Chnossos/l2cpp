/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// ReSharper disable CppNonExplicitConvertingConstructor

// Project includes
#include "../../../../game/constants/SystemMessageId.hpp"
#include "../i18n/SystemMessageArgument.hpp"

class Actor;

namespace Network::Packet::Server { class ChatSystemSayPacket; }

class Network::Packet::Server::ChatSystemSayPacket final : public l2cpp::Network::Packet
{
public:
    explicit ChatSystemSayPacket(SystemMessageId messageId);

public:
    template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
    ChatSystemSayPacket & appendArg(T && arg) { return appendArgImpl(arg); }

    ChatSystemSayPacket & appendName(Actor const &);

private:
    ChatSystemSayPacket & appendArgImpl(SystemMessageArgument const & arg);

private:
    u32 _argsCountOffset;
};

template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
Network::Packet::Server::ChatSystemSayPacket & operator<<(Network::Packet::Server::ChatSystemSayPacket & p, T && arg) {
    return p.appendArg(std::forward<T>(arg));
}
