/// @author    Chnossos
/// @date      Created on 2026-02-27

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/components/KnownActors.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/world/GameObjectDeletePacket.hpp>

// C++ includes
#include <unordered_set>

DEFINE_PACKET_HANDLER(MoveUpdate)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    s32 x, y, z;
    u32 orientation;
    reader >> x >> y >> z >> orientation;

    if (orientation > std::numeric_limits<u16>::max())
        SPDLOG_WARN("Character's orientation ('{}') is greater than its limit!", orientation);

    auto & c = *player.currentCharacter();
    c.setPosition(x, y, z);
    c.setOrientation(static_cast<u16>(orientation));

    std::unordered_set<GameObjectId> surroundingActorIds;
    World::forEachActorAround(c, [&] (auto const & actor) { surroundingActorIds.emplace(actor.id()); });

    auto & knownActors = c.getOrAddComponent<KnownActors>().ids;
    // First, delete from client all actors that are now too far away, and skip those that are still there
    for (auto it = knownActors.begin(); it != knownActors.end(); )
    {
        if (!surroundingActorIds.contains(*it))
        {
            player.connection().send(GameObjectDeletePacket{*it});
            it = knownActors.erase(it);
        }
        else
            surroundingActorIds.erase(*it++);
    }

    // Remaining actors are notified and merged
    for (auto const actorId : surroundingActorIds)
        World::sendStatus(actorId, player);

    knownActors.merge(surroundingActorIds);
}
