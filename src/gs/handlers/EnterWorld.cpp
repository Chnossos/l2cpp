/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/actor/Npc.hpp>
#include <gs/game/components/ActorAutoRegen.hpp>
#include <gs/game/components/KnownActors.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/game/spawn/SpawnManager.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/client/ClientForceDisconnectPacket.hpp>
#include <gs/network/packets/server/inventory/InventoryListPacket.hpp>
#include <gs/network/packets/server/status/ActorDiePacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdateBroadcastPacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdatePacket.hpp>
#include <gs/network/packets/server/ui/UiShortcutListPacket.hpp>

DEFINE_PACKET_HANDLER(EnterWorld) try
{
    auto & c    = *player.currentCharacter();
    auto & conn = player.connection();

    c.stats().compute(c);
    if (c.isAlive())
        c.addComponent<ActorAutoRegen>();

    conn.send(InventoryListPacket{false, c.inventory()});
    conn.send(UiShortcutListPacket{c.shortcutBar()});
    conn.send(ChatSystemSayPacket{SystemMessageId::WelcomeToTheWorldOfL2});

    // Send surrounding actors
    auto & knownActors = c.getOrAddComponent<KnownActors>().ids;
    World::forEachActorAround(c, [&] (Actor & a)
    {
        World::sendStatus(a, player);
        knownActors.emplace(a.id());

        if (a.type() == ActorType::Character)
            a.getOrAddComponent<KnownActors>().ids.emplace(c.id());
    });

    // End the loading screen
    conn.send(CharacterStatusUpdatePacket(c));

    if (!c.isAlive())
        conn.send(ActorDiePacket{c});

    // Finally, show to other players
    World::broadcastAround(c, CharacterStatusUpdateBroadcastPacket(c));
}
catch (...)
{
    // Any exception would get the client stuck during the login screen; force disconnect him instead.
    player.connection().send(ClientForceDisconnectPacket());
    player.connection().close();
    throw;
}
