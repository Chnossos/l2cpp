/// @author    Chnossos
/// @date      Created on 2026-06-11

#pragma once

// Project includes
#include <gs/game/actions/Action.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/constants/ActorState.hpp>
#include <gs/network/packets/server/movement/ObjectTeleportPacket.hpp>

class TeleportAction final : public Action
{
public:
    TeleportAction(Actor & performer, Position destination, TeleportationStyle, bool sendSystemMessage = false);

public:
    auto canBeInterruptedByAnotherAction() const -> bool override    { return false;        }
    auto destination()                     const -> Position const & { return _destination; }

private:
    void onStarted() override;
    void updateImpl(ClockDuration) override;
    void onFinished() override;

private:
    Position           _destination;
    TeleportationStyle _style;
    bool               _sendSystemMessage;
    ActorState         _previousState;
};
