/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "ChatSystemSayPacket.hpp"

// Project includes
#include <gs/game/actor/Npc.hpp>
#include <gs/game/components/NpcAppearance.hpp>
#include <gs/game/skill/Skill.hpp>

using Network::Packets::Server::ChatSystemSayPacket;

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

ChatSystemSayPacket & ChatSystemSayPacket::appendName(Skill const & skill)
{
    return appendName(skill.tmplate());
}

ChatSystemSayPacket & ChatSystemSayPacket::appendName(SkillUid const uid)
{
    appendArgImpl(SysMsgArg::SkillName{uid});
    return *this;
}

ChatSystemSayPacket & ChatSystemSayPacket::appendName(SkillTemplate const & skill)
{
    return appendName(skill.uid());
}

auto ChatSystemSayPacket::argumentCount() const -> size_t
{
    return counterAtOffset(_argsCountOffset);
}

ChatSystemSayPacket & ChatSystemSayPacket::appendArgImpl(SystemMessageArgument const & arg)
{
    counterAtOffset(_argsCountOffset) += 1;
    arg.serialize(*this);
    return *this;
}
