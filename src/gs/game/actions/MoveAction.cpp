/// @author    Chnossos
/// @date      Created on 2026-03-24

#include "MoveAction.hpp"

// Project includes
#include <gs/Player.hpp>
#include <gs/game/World.hpp>
#include <gs/game/actor/Actor.hpp>
#include <gs/network/Connection.hpp>
#include <gs/network/packets/server/movement/ActorMovePacket.hpp>
#include <gs/network/packets/server/movement/ActorMoveStopPacket.hpp>

MoveAction::MoveAction(Actor & performer, Position origin, Position target, Input const input)
    : Action{ActionType::Move, performer}
    , _origin{std::move(origin)}
    , _target{std::move(target)}
    , _input{input}
    , _previousState{}
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
