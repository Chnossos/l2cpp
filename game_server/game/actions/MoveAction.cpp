/// @author    Chnossos
/// @date      Created on 2026-03-24

#include "MoveAction.hpp"

// Project includes
#include "../../Player.hpp"
#include "../../network/Connection.hpp"
#include "../../network/packets/server/movement/ActorMovePacket.hpp"
#include "../../network/packets/server/movement/ActorMoveStopPacket.hpp"
#include "../World.hpp"
#include "../actor/Actor.hpp"

MoveAction::MoveAction(Actor & performer, Position const & origin, Position const & target, Input const input)
    : Action(ActionType::Move, performer)
    , _origin(origin)
    , _target(target)
    , _input(input)
{}

bool MoveAction::canBeInterruptedByAnotherAction() const
{
    return true;
}

void MoveAction::onStarted()
{
    _previousState = performer().state;
    performer().state = ActorState::Moving;
    performer().setPosition(_origin);

    // Make character start moving, position will be validated in MoveUpdate handler
    World::broadcastAround(performer(), Network::Packets::Server::ActorMovePacket{performer(), _origin, _target}, true);
}

void MoveAction::updateImpl(ClockDuration)
{
    setFinished(_currentDistance >= _totalDistance);
}

void MoveAction::onFinished()
{
    performer().state = _previousState;
}

void MoveAction::onCanceled()
{
    performer().state = _previousState;
    World::broadcastAround(performer(), Network::Packets::Server::ActorMoveStopPacket{performer()}, true);
}
