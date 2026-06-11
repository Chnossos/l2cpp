/// @author    Chnossos
/// @date      Created on 2026-03-31

#pragma once

// ReSharper disable CppNonExplicitConvertingConstructor

// Project includes
#include <gs/game/constants/SystemMessageId.hpp>
#include <gs/network/packets/server/i18n/SystemMessageArgument.hpp>
#include <gs/utils/Conversion.hpp>

class Actor;
class Skill;
class SkillTemplate;

namespace Network::Packets::Server { class ChatSystemSayPacket; }

class Network::Packets::Server::ChatSystemSayPacket final : public Packet
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

public:
    auto argumentCount() const -> size_t;

private:
    ChatSystemSayPacket & appendArgImpl(SystemMessageArgument const & arg);

private:
    u32 _argsCountOffset;
};

template<class T> requires std::is_base_of_v<SystemMessageArgument, T>
Network::Packets::Server::ChatSystemSayPacket & operator<<(Network::Packets::Server::ChatSystemSayPacket & p, T && arg) {
    return p.appendArg(std::forward<T>(arg));
}

inline Network::Packets::Server::ChatSystemSayPacket & operator<<(Network::Packets::Server::ChatSystemSayPacket & p,
                                                                  std::string const & arg)
{
    return p.appendArg(SysMsgArg::Text{Utils::toWideString(arg)});
}

inline Network::Packets::Server::ChatSystemSayPacket & operator<<(Network::Packets::Server::ChatSystemSayPacket & p,
                                                                  std::wstring const & arg)
{
    return p.appendArg(SysMsgArg::Text{arg});
}
