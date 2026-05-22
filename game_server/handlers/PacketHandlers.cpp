/// @author    Chnossos
/// @date      Created on 2026-02-24

#include "PacketHandlers.hpp"

// Project includes
#include "../network/packets/client/OpCodes.hpp"

namespace Handler
{
#define HANDLER(name) void handle ## name(Player &);
    HANDLER(ActionRequest)
    HANDLER(Authenticate)
    HANDLER(CharacterCancelDeletion)
    HANDLER(CharacterCreate)
    HANDLER(CharacterSelect)
    HANDLER(CharacterShowCreationScreen)
    HANDLER(ChatAdminCommand)
    HANDLER(ChatSay)
    HANDLER(ClientRestart)
    HANDLER(ConnectionClosing)
    HANDLER(EnterWorld)
    HANDLER(InventoryOpen)
    HANDLER(ItemUnequip)
    HANDLER(ItemUse)
    HANDLER(ManorList)
    HANDLER(MiniMapShow)
    HANDLER(Move)
    HANDLER(MoveUpdate)
    HANDLER(Protocol)
    HANDLER(QuestList)
    HANDLER(SkillList)
    HANDLER(SkillUse)
    HANDLER(SocialActionPerform)
    HANDLER(TargetClear)
    HANDLER(UiConfirmationModalAnswer)
    HANDLER(UiShortcutSet)
    HANDLER(UiShortcutUnset)
#undef HANDLER
}

std::unordered_map<byte, std::pair<PacketHandler, std::string_view>> gPacketHandlers
{
#define HANDLER(name) {std::to_underlying(Network::ClientOpCode::name), {&Handler::handle ## name, #name}}
    HANDLER(ActionRequest),
    HANDLER(Authenticate),
    HANDLER(CharacterCancelDeletion),
    HANDLER(CharacterCreate),
    HANDLER(CharacterSelect),
    HANDLER(CharacterShowCreationScreen),
    HANDLER(ChatAdminCommand),
    HANDLER(ChatSay),
    HANDLER(ClientRestart),
    HANDLER(ConnectionClosing),
    HANDLER(EnterWorld),
    HANDLER(InventoryOpen),
    HANDLER(ItemUnequip),
    HANDLER(ItemUse),
    HANDLER(ManorList),
    HANDLER(MiniMapShow),
    HANDLER(Move),
    HANDLER(MoveUpdate),
    HANDLER(Protocol),
    HANDLER(QuestList),
    HANDLER(SkillList),
    HANDLER(SkillUse),
    HANDLER(SocialActionPerform),
    HANDLER(TargetClear),
    HANDLER(UiConfirmationModalAnswer),
    HANDLER(UiShortcutSet),
    HANDLER(UiShortcutUnset),
#undef HANDLER
};
