/// @author    Chnossos
/// @date      Created on 2026-03-04

// Project includes
#include <common/utils/Enum.hpp>
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/Gear.hpp>
#include <gs/game/inventory/ItemStorage.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/inventory/InventoryUpdatePacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdateBroadcastPacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdatePacket.hpp>

// C++ includes
#include <ranges>

DEFINE_PACKET_HANDLER(ItemUse)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    GameObjectId uid;
    bool ctrlPressed;
    reader >> uid >> ctrlPressed;

    auto & character = *player.currentCharacter();
    auto const item  = character.inventory().item(uid);
    if (!item)
        return;

    switch (item->tmplate.category)
    {
        case ItemCategory::Weapon:
        case ItemCategory::Armor:
        case ItemCategory::Accessory:
        {
            if (auto const transaction = character.gear().equipItem(*item); transaction.succeeded)
            {
                InventoryUpdatePacket p;
                if (!transaction.oldItems.empty())
                {
                    for (Item const & oldItem : transaction.oldItems | std::views::values)
                    {
                        p.appendModifiedItem(oldItem);

                        using enum ItemCategory;
                        if (Utils::Enum::isAnyOf(oldItem.tmplate.category, Armor, Accessory, Weapon))
                        {
                            ChatSystemSayPacket msg{SystemMessageId::_1_HasBeenDisarmed};
                            msg.appendArg(SysMsgArg::ItemName{oldItem.tmplate.id});
                            player.connection().send(msg);
                        }
                    }
                }

                if (transaction.curItem)
                {
                    p.appendModifiedItem(*transaction.curItem);

                    ChatSystemSayPacket msg{SystemMessageId::YouAreEquippedWith_1};
                    msg.appendArg(SysMsgArg::ItemName{transaction.curItem->tmplate.id});
                    player.connection().send(msg);
                }

                if (item->tmplate.id == 6619) // FIXME: if any bow, find matching arrows to equip in left hand
                {
                    if (auto const matchingArrows = character.inventory().item({.id = 1345}); !matchingArrows.empty())
                    {
                        auto const subTr = character.gear().equipItem(matchingArrows.front());
                        p.appendModifiedItem(*subTr.curItem);
                    }
                }

                player.connection().send(p);
                player.connection().send(CharacterStatusUpdatePacket{character});
                World::broadcastAround(character, CharacterStatusUpdateBroadcastPacket{character});
            }
            break;
        }
        case ItemCategory::Misc:
            // Do something if item is usable (e.g. potions)
            break;

        default:
            break;
    }
}
