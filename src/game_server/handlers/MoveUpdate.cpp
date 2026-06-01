/// @author    Chnossos
/// @date      Created on 2026-02-27

// Project includes
#include "../game/actor/Character.hpp"
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(MoveUpdate)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    s32 x, y, z, orientation;
    reader >> x >> y >> z >> orientation;

    auto & c = *player.currentCharacter();
    c.setPosition(x, y, z);
    c.setOrientation(orientation);

    // if (player.actions().empty())
    // {
    //     SPDLOG_WARN("MoveUpdate called with no prior Move");
    //     return;
    // }
    //
    // SPDLOG_DEBUG("Client says he's at x={} y={}", x, y);
    //
    // auto & move = static_cast<MoveAction &>(*player.actions().front());
    //
    // auto const now             = std::chrono::steady_clock::now();
    // auto const elapsed         = std::chrono::duration_cast<std::chrono::milliseconds>(now - move.lastUpdateTime);
    // auto const unitSpeedPerMs  = c.stats().runSpeed / 1000.f;
    // auto const distanceCovered = elapsed.count() * unitSpeedPerMs;
    //
    // move.currentDistance += distanceCovered;
    // SPDLOG_DEBUG("I have covered {} units out of {} total in {:%Q%q}", move.currentDistance, move.totalDistance,
    //              std::chrono::duration_cast<std::chrono::milliseconds>(now - move.lastUpdateTime));
    //
    // move.lastUpdateTime = now;
    // if ((move.currentDistance/* += distanceCovered*/) >= move.totalDistance) // Reached destination
    // {
    //     Packet p(0x61);
    //     p << c.id() << x << y << c.position().z << c.position().orientation;
    //     player.connection().send(p);
    // }
}
