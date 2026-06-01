/// @author    Chnossos
/// @date      Created on 2026-03-03

// Project includes
#include "_Common.hpp"
#include "../game/World.hpp"
#include "../game/actor/Character.hpp"
#include "../network/packets/server/chat/ChatSayPacket.hpp"

using Network::Packets::Server::ChatSayPacket;

DEFINE_PACKET_HANDLER(ChatSay)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    std::wstring msg, recipient;
    ChatType type;
    reader >> msg >> type;

    if (type == ChatType::Whisper)
        reader >> recipient;

    SPDLOG_DEBUG(L"Player said ('{}'){}: “{}”",
                 std::to_underlying(type), recipient.empty() ? L"" : std::format(L" (to '{}')", recipient), msg);

    auto const & c = *player.currentCharacter();
    if (recipient.empty())
        World::broadcast(ChatSayPacket(c.id(), c.name(), type, msg));
    else
        player.connection().send(ChatSayPacket(c.id(), L"->" + recipient, type, msg));
}
