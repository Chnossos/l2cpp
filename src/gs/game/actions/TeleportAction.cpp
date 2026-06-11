/// @author    Chnossos
/// @date      Created on 2026-06-11

#include "TeleportAction.hpp"

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actor/Actor.hpp>
#include <gs/game/components/KnownActors.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>

TeleportAction::TeleportAction(
    Actor                    & performer,
    Position                   destination,
    TeleportationStyle const   style,
    bool               const   sendSystemMessage
)
    : Action{ActionType::Teleport, performer}
    , _destination{std::move(destination)}
    , _style{style}
    , _sendSystemMessage{sendSystemMessage}
    , _previousState{performer.state}
{}

void TeleportAction::onStarted()
{
    auto & a = performer();

    _previousState = std::exchange(a.state, ActorState::Teleporting);

    if (_sendSystemMessage)
    {
        Network::Packets::Server::ChatSystemSayPacket msg{SystemMessageId::_1_2};
        msg << "Teleporting to:"
            << std::format(R"(x: {} ; y: {} ; z: {})", _destination.x, _destination.y, _destination.z);

        World::send(a, msg);
    }

    // Keep same orientation
    _destination.orientation = a.position().orientation;

    // Notify performer and onlookers
    World::broadcastAround(a, Network::Packets::Server::ObjectTeleportPacket{a.id(), _destination, _style}, true);
}

void TeleportAction::updateImpl(ClockDuration)
{
    setFinished(performer().position() == _destination);
}

void TeleportAction::onFinished()
{
    auto & emitter = performer();

    if (auto const knownActors = emitter.component<KnownActors>())
        knownActors->ids.clear();

    World::forEachActorAround(emitter, [&] (Actor & actor)
    {
        if (auto const knownActors = emitter.component<KnownActors>())
            knownActors->ids.emplace(actor.id());

        World::sendStatus(actor, emitter);

        if (auto const knownActors = actor.component<KnownActors>())
            knownActors->ids.emplace(emitter.id());

        World::sendStatus(emitter, actor);
    });
    emitter.state = _previousState;
}
