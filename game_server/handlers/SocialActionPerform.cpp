/// @author    Chnossos
/// @date      Created on 2026-05-22

#pragma once

// Project includes
#include "../game/actions/SocialAction.hpp"
#include "../game/actor/Character.hpp"
#include "../network/packets/server/action/ActionFailedPacket.hpp"
#include "_Common.hpp"

#include <l2cpp/utils/Enum.hpp>

DEFINE_PACKET_HANDLER(SocialActionPerform) try
{
    L2CPP_B_ASSERT(player.currentCharacter(), "Cannot perform social action, no current character");

    PacketReader reader(player.connection().readBuffer().subspan(3));

    SocialActionId actionId;
    reader >> actionId;

    using enum SocialActionId;
    L2CPP_B_ASSERT(l2cpp::Utils::Enum::isInContiguousInclusiveRange(actionId, Hello, Sad),
                   "Invalid social action id '{}'", std::to_underlying(actionId));

    player.currentCharacter()->doNext<SocialAction>(actionId);
}
catch (l2cpp::Exception const & e)
{
    SPDLOG_ERROR("Failed to perform social action:\n{}", l2cpp::formatExceptionStack(e));
    player.connection().send(ActionFailedPacket{});
}
