/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// ReSharper disable CppNonExplicitConvertingConstructor

// Project includes
#include <gs/game/constants/SystemMessageId.hpp>
#include <gs/network/packets/server/i18n/SystemMessageArgument.hpp>

class Actor;
class Skill;
class SkillTemplate;

namespace Network::Packets::Server { class ChatSystemSayPacket; }

class Network::Packets::Server::ChatSystemSayPacket final : public Network::Packet
{
public:
    explicit ChatSystemSayPacket(SystemMessageId messageId);

public:
    template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
    ChatSystemSayPacket & appendArg(T && arg) { return appendArgImpl(arg); }

    ChatSystemSayPacket & appendName(Actor const &);
    ChatSystemSayPacket & appendName(Skill const &);
    ChatSystemSayPacket & appendName(SkillUid);
    ChatSystemSayPacket & appendName(SkillTemplate const &);

private:
    ChatSystemSayPacket & appendArgImpl(SystemMessageArgument const & arg);

private:
    u32 _argsCountOffset;
};

template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
Network::Packets::Server::ChatSystemSayPacket & operator<<(Network::Packets::Server::ChatSystemSayPacket & p, T && arg) {
    return p.appendArg(std::forward<T>(arg));
}
