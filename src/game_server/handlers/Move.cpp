/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "../game/actions/MoveAction.hpp"
#include "../game/actor/Character.hpp"
#include "_Common.hpp"

// Taken from Ruk33
// ================
// float const distanceX = targetX - c.position().x;
// float const distanceY = targetY - c.position().y;
// float const distanceZ = targetZ - c.position().z;
//
// float const distance = sqrt(distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ);
// float const speed    = c.stats()[StatId::RunSpeed];
//
// float const ticksPerSecond = 10;
// u64   const ticksToMove    = ticksPerSecond * distance / speed;
//
// float const sin = distanceY / distance;
// float const cos = distanceX / distance;
//
// auto const xSpeedTicks = cos * speed / ticksPerSecond;
// auto const ySpeedTicks = sin * speed / ticksPerSecond;
//
// targetX = c.position().x + static_cast<s32>(distance * cos);
// targetY = c.position().y + static_cast<s32>(distance * sin);

DEFINE_PACKET_HANDLER(Move)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    Position origin, target;
    MoveAction::Input input;
    reader >> target >> origin >> input;

    // TODO: trust originX/Y/Z only if they are close enough to server position

    // SPDLOG_TRACE("Moving requested to {{x={}, y={}, z={}}} (input: {})",
    //              action.targetX, action.targetY, action.targetZ,
    //              (action.input == *MoveAction::Input::Mouse ? "mouse" : "keyboard"));
    //
    auto & c = *player.currentCharacter();
    // float const distanceX = action.targetX - c.position().x;
    // float const distanceY = action.targetY - c.position().y;
    // float const distanceZ = action.targetZ - c.position().z;
    // action.totalDistance = std::sqrt(distanceX * distanceX + distanceY * distanceY/* + distanceZ * distanceZ*/);
    //
    // float const distanceCoveredInOneMs = c.stats()[StatId::RunSpeed] / 1000.f;
    //
    // std::chrono::duration<float, std::milli> const totalDuration(action.totalDistance / distanceCoveredInOneMs);
    // SPDLOG_TRACE("Character must travel through {} units. At a speed of {} units per 1s, it will take {:%Q%q}",
    //              action.totalDistance, c.stats()[StatId::RunSpeed],
    //              std::chrono::duration_cast<std::chrono::milliseconds>(totalDuration));
    //
    c.doNext<MoveAction>(origin, target, input);
}
