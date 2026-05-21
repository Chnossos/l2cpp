/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "ChatSystemSayPacket.hpp"

// Project includes
#include "../../../../game/actor/Npc.hpp"
#include "../../../../game/components/NpcAppearance.hpp"

using Network::Packet::Server::ChatSystemSayPacket;

ChatSystemSayPacket::ChatSystemSayPacket(SystemMessageId const messageId)
    : Packet(0x64, "ChatSystemSay")
{
    *this << messageId;

    appendCounterAndStoreOffset(_argsCountOffset);
}

ChatSystemSayPacket & ChatSystemSayPacket::appendName(Actor const & actor)
{
    if (actor.type() == ActorType::Character)
        appendArgImpl(SysMsgArg::Text{actor.name()});
    else
        appendArgImpl(SysMsgArg::NpcName{static_cast<Npc const &>(actor).appearance().id()});

    return *this;
}

ChatSystemSayPacket & ChatSystemSayPacket::appendArgImpl(SystemMessageArgument const & arg)
{
    counterAtOffset(_argsCountOffset) += 1;
    arg.serialize(*this);
    return *this;
}
