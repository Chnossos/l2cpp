/// @author    Chnossos
/// @date      Created on 2026-03-03

#pragma once

// Project includes
#include "../../Typedefs.hpp"

enum class ChatType : u32
{
    General,              ///< In-game format: @c <message>
    Shout,                ///< In-game format: @c !<message>
    Whisper,              ///< In-game format: @code "<recipient> <message>
    Party,                ///< In-game format: @c #<message>
    Clan,                 ///< In-game format: @c @<message>
    Gm,
    PetitionPlayer,       ///< In-game format: @c &<message>
    PetitionGm,
    Trade,                ///< In-game format: @c +<message>
    Alliance,             ///< In-game format: @c $<message>
    Announcement,
    Boat,
    Friend,
    MsnChat,              ///< Unused.
    PartyMatchingRoom,
    ChannelCommanders,
    ChannelCommand,
    Hero,                 ///< In-game format: @c %<message>
    CriticalAnnouncement,
    ScreenAnnouncement,
    Battlefield,
    MpccRoom,             ///< Unknown.

    Count,
};
