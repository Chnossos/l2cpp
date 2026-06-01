/// @author    Chnossos
/// @date      Created on 2026-04-26

#pragma once

// Project includes
#include "_Common.hpp"
#include "../game/actor/Character.hpp"

DEFINE_PACKET_HANDLER(UiConfirmationModalAnswer)
{
    L2CPP_B_ASSERT(player.currentCharacter(), "Cannot handle confirmation modal answer: no current character");

    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 systemMessageId, answer;
    reader >> systemMessageId >> answer;

    player.currentCharacter()->answerConfirmationModal(systemMessageId, answer);
}
