/// @author    Chnossos
/// @date      Created on 2026-05-22

// Project includes
#include <common/utils/Enum.hpp>
#include <gs/game/actions/SocialAction.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/action/ActionFailedPacket.hpp>

DEFINE_PACKET_HANDLER(SocialActionPerform) try
{
    L2CPP_B_ASSERT(player.currentCharacter(), "Cannot perform social action, no current character");

    PacketReader reader(player.connection().readBuffer().subspan(3));

    SocialActionId actionId;
    reader >> actionId;

    using enum SocialActionId;
    L2CPP_B_ASSERT(Utils::Enum::isInContiguousInclusiveRange(actionId, Hello, Sad),
                   "Invalid social action id '{}'", std::to_underlying(actionId));

    player.currentCharacter()->doNext<SocialAction>(actionId);
}
catch (Core::Exception const & e)
{
    SPDLOG_ERROR("Failed to perform social action:\n{}", Core::formatExceptionStack(e));
    player.connection().send(ActionFailedPacket{});
}
