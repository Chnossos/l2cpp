/// @author    Chnossos
/// @date      Created on 2026-04-26

#include "UiConfirmationModalShowPacket.hpp"

using Network::Packets::Server::UiConfirmationModalShowPacket;

UiConfirmationModalShowPacket::UiConfirmationModalShowPacket(u32 const systemMessageId)
    : Packet(0xed, "UiConfirmationModalShow")
{
    *this << systemMessageId;
    appendCounterAndStoreOffset(_argsCountOffset);
}

UiConfirmationModalShowPacket & UiConfirmationModalShowPacket::appendArgImpl(SystemMessageArgument const & arg)
{
    counterAtOffset(_argsCountOffset) += 1;
    arg.serialize(*this);
    return *this;
}
