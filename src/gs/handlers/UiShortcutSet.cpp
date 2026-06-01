/// @author    Chnossos
/// @date      Created on 2026-03-02

// Project includes
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/SkillDirectory.hpp>
#include <gs/game/inventory/ItemStorage.hpp>
#include <gs/game/ui/ActionShortcut.hpp>
#include <gs/game/ui/ItemShortcut.hpp>
#include <gs/game/ui/ShortcutBar.hpp>
#include <gs/game/ui/SkillShortcut.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/ui/UiShortcutSetPacket.hpp>

DEFINE_PACKET_HANDLER(UiShortcutSet)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 type, index;
    reader >> type >> index;

    auto & c = *player.currentCharacter();

    switch (static_cast<ShortcutType>(type))
    {
        case ShortcutType::Item:
        {
            GameObjectId itemId;
            reader >> itemId;

            if (auto const item = c.inventory().item(itemId))
                c.shortcutBar().set<ItemShortcut>(index, item);
            else
                SPDLOG_ERROR("Invalid item shortcut: item '{}' not found inside inventory", itemId);

            break;
        }

        case ShortcutType::Skill:
        {
            u32 skillId;
            reader >> skillId;

            if (auto const skill = c.skills().skill(static_cast<SkillId>(skillId)))
                c.shortcutBar().set<SkillShortcut>(index, skill->tmplate().uid());
            else
                SPDLOG_ERROR("Invalid skill shortcut: skill '{}' not learned", skillId);

            break;
        }

        case ShortcutType::Action:
        {
            u32 actionId;
            reader >> actionId;
            c.shortcutBar().set<ActionShortcut>(index, actionId);
            break;
        }

        case ShortcutType::Macro:
            return;

        case ShortcutType::Recipe:
            return;

        default:
            SPDLOG_ERROR("Unknown shortcut type '{}'", type);
            return;
    }

    player.connection().send(UiShortcutSetPacket{c.shortcutBar().at(index)});
}
