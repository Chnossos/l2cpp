/// @author    Chnossos
/// @date      Created on 2026-03-16

#pragma once

// Project includes
#include <gs/Typedefs.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/constants/CharacterCreationResult.hpp>

// C++ includes
#include <functional>
#include <list>
#include <source_location>
#include <unordered_map>
#include <vector>

class Actor;
class Character;
class Monster;
class Npc;
class Player;
struct CharacterCreationParameters;
struct Loot;
struct System;

namespace Network { class Packet; }

class World
{
public:
    World() noexcept = delete;

public:
    static void init();

public:
    static auto actor    (GameObjectId) -> OptRef<Actor>;
    static auto character(GameObjectId) -> OptRef<Character>;
    static auto monster  (GameObjectId) -> OptRef<Monster>;
    static auto npc      (GameObjectId) -> OptRef<Npc>;

public:
    static void update(ClockDuration elapsed);

    template<typename T, typename... Args> requires std::is_base_of_v<System, T>
    static void registerSystem(Args &&... args) {
        _systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    static auto createCharacter(Player const &, CharacterCreationParameters const &) -> CharacterCreationResult;
    static auto getCharacterPreviews(Player const &) -> std::vector<Ref<Character>>;
    static auto addCharacterPreview(AccountId, CharacterCreationParameters const &) -> Character &;
    static auto loadCharacterFromPreview(Character const &) -> Character &;
    static void moveCharacterBackToPreviews(Character &);
    static void delCharacterPreview(AccountId, size_t index);

    static auto addCharacter(CharacterCreationParameters const &, OptRef<Player> = std::nullopt) -> Character &;
    static void initCharacter(Character &);
    static auto addNpc(u32 id, Position const & = Position{}) -> OptRef<Npc>;

    static void scheduleForDeletion(Actor &, ClockDuration timeFromNow = ClockDuration::zero());
    static void unscheduleForDeletion(Actor &);

    /// @returns The amount of seconds elasped since in-game midnight.
    static auto inGameTime() -> std::chrono::minutes;

    static auto subscribeToTarget(GameObjectId targetId, Actor const & listener) -> OptRef<Actor>;
    static void subscribeToTarget(Actor const & target, Actor const & listener);
    static void unsubscribeFromTarget(Actor const & target, Actor const & listener);
    static void unsubscribeAllTargetListeners(Actor const & target);

    static void distributeLoot(Loot const &, DamageDealtTable const & attackerDamageAmounts);

    static void forEachActorAround(Actor const & source, std::function<void(Actor &)> const &);

    static bool isInBroadcastRange(Actor const & source, Actor const & target);

    /// Sends a packet to specified actor if it is a Character with an active player
    static void send(Actor const & to, Network::Packet & packet,
                     std::source_location const & src = std::source_location::current());

    /// Sends a packet to specified actor if it is a Character with an active player
    static void send(Actor const & to, Network::Packet && packet,
                     std::source_location const & src = std::source_location::current()) { send(to, packet, src); }

    static void sendStatus(Actor const & from, Actor const & to,
                           std::source_location const & src = std::source_location::current());

    static void sendStatus(Actor const & from, Player & to,
                           std::source_location const & src = std::source_location::current());

    static void sendStatus(GameObjectId id, Player & to,
                           std::source_location const & src = std::source_location::current());

    /// @warning This broadcasts given packet to ALL online players, regardless of distance!
    static void broadcast(Network::Packet &&, std::source_location const & src = std::source_location::current());

    /// Broadcasts given packet to all online players around emitter, including the emitter if needed
    static void broadcastAround(Actor const & emitter, Network::Packet &&, bool includeEmitter = false,
                                std::source_location const & src = std::source_location::current());

    /// Broadcasts given packet to all online players whose current target is the emitter
    static void broadcastToSubscribers(Actor const & emitter, Network::Packet &&, bool includeEmitter = false,
                                       std::source_location const & src = std::source_location::current());

private:
    template<class T, typename... Args> requires std::is_base_of_v<Actor, T>
    static auto addActor(Args &&... args) -> T & {
        return static_cast<T &>(addActor(std::make_unique<T>(std::forward<Args>(args)...)));
    }
    static auto addActor(std::unique_ptr<Actor> actor) -> Actor &;
    static void delActor(Actor & a);

private:
    static std::vector<std::unique_ptr<System>> _systems;

    static std::unordered_map<AccountId,    std::vector<GameObjectId>>  _characterPreviewsIndex;
    static std::unordered_map<GameObjectId, std::unique_ptr<Character>> _characterPreviews;
    static std::unordered_map<GameObjectId, std::unique_ptr<Actor>>     _actors;

    static std::unordered_map<GameObjectId, Ref<Actor>> _scheduledForDeletion;

    static std::unordered_map<GameObjectId, std::list<GameObjectId>> _targetSubscribers;
};
