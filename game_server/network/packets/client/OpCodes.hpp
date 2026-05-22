/// @author    Chnossos
/// @date      Created on 2026-03-03

#pragma once

// Project includes
#include "../Typedefs.hpp"

namespace Network
{
    enum class ClientOpCode : byte
    {
        Protocol                    = 0x00,
        Move                        = 0x01,
        EnterWorld                  = 0x03,
        ActionRequest               = 0x04,
        Authenticate                = 0x08,
        ConnectionClosing           = 0x09,
        CharacterCreate             = 0x0b,
        CharacterSelect             = 0x0d,
        CharacterShowCreationScreen = 0x0e,
        InventoryOpen               = 0x0f,
        ItemUnequip                 = 0x11,
        ItemUse                     = 0x14,
        SocialActionPerform         = 0x1b,
        SkillUse                    = 0x2f,
        UiShortcutSet               = 0x33,
        UiShortcutUnset             = 0x35,
        TargetClear                 = 0x37,
        ChatSay                     = 0x38,
        SkillList                   = 0x3f,
        ClientRestart               = 0x46,
        MoveUpdate                  = 0x48,
        ChatAdminCommand            = 0x5b,
        CharacterCancelDeletion     = 0x62,
        QuestList                   = 0x63,
        UiConfirmationModalAnswer   = 0xc5,
        MiniMapShow                 = 0xcd,
        ManorList                   = 0xd0,
    };
}
