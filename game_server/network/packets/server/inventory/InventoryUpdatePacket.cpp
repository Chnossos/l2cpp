/// @author    Chnossos
/// @date      Created on 2026-03-06

#include "InventoryUpdatePacket.hpp"

using Network::Packets::Server::InventoryUpdatePacket;

namespace
{
    enum class UpdateType : u16 { Added = 1, Modified, Removed };

    void append(InventoryUpdatePacket & packet, UpdateType const type, Item const & item)
    {
        *reinterpret_cast<u16 *>(packet.body().data() + sizeof(u8)) += 1;
        packet << std::to_underlying(type) << item;
    }
}

InventoryUpdatePacket::InventoryUpdatePacket()
    : Packet(0x27, "InventoryUpdate")
{
    *this << 0_u16;
}

void InventoryUpdatePacket::appendAddedItem(Item const & item)
{
    ::append(*this, UpdateType::Added, item);
}

void InventoryUpdatePacket::appendModifiedItem(Item const & item)
{
    ::append(*this, UpdateType::Modified, item);
}

void InventoryUpdatePacket::appendRemovedItem(Item const & item)
{
    ::append(*this, UpdateType::Removed, item);
}
