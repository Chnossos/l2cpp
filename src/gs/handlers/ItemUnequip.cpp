/// @author    Chnossos
/// @date      Created on 2026-03-06

// Project includes
#include <common/utils/Enum.hpp>
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/Gear.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/inventory/InventoryUpdatePacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdateBroadcastPacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdatePacket.hpp>

// C++ includes
#include <ranges>

DEFINE_PACKET_HANDLER(ItemUnequip)
{
    L2CPP_B_ASSERT(player.currentCharacter(), "No current player, can't unequip item");

    PacketReader reader(player.connection().readBuffer().subspan(3));

    GearSlot slot;
    reader >> slot;

    auto & c = *player.currentCharacter();
    if (auto const item = c.gear().item(slot); item)
    {
        // FIXME: if any kind of arrow
        Ref const finalitem = item->tmplate.id == 1345 ? *c.gear().item(GearSlot::RightHand) : *item;

        if (auto const transaction = c.gear().unequipItem(finalitem); transaction.succeeded)
        {
            InventoryUpdatePacket p;

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

            player.connection().send(p);
            player.connection().send(CharacterStatusUpdatePacket{c});
            World::broadcastAround(c, CharacterStatusUpdateBroadcastPacket{c});
        }
    }
}
