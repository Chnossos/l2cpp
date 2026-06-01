/// @author    Chnossos
/// @date      Created on 2026-03-15

#pragma once

// Project includes
#include "../GameObject.hpp"
#include "../actions/Action.hpp"
#include "../constants/ActorState.hpp"
#include "../constants/ActorType.hpp"
#include "../constants/Team.hpp"
#include "../ecs/Entity.hpp"
#include "../effects/Effect.hpp"

#include <l2cpp/Event.hpp>
#include <l2cpp/Pimpl.hpp>

class Gear;
class SkillDirectory;
struct ComputedStats;
struct Position;
struct Stats;

/// Base class for player characters, npcs, summons, pets…
class Actor : public GameObject, public Entity
{
public:
    ActorState state = ActorState::Idle;

public:
    explicit Actor(ActorType type);
    Actor(Actor &&) noexcept;
    Actor & operator=(Actor &&) noexcept;
    ~Actor() override = 0;

public:
    bool operator==(Actor const &) const;

public:
    Event<> onAbnormalEffectListChanged;
    Event<> onDied;
    Event<> onLeveledUp;
    Event<> onRevived;

public:
    auto type()     const -> ActorType;
    auto name()     const -> std::wstring_view;
    auto title()    const -> std::wstring_view;
    auto position() const -> Position const &;
    auto team()     const -> Team;

    auto stats()       -> Stats       &;
    auto stats() const -> Stats const &;

    auto gear()       -> Gear       &;
    auto gear() const -> Gear const &;

    auto skills()       -> SkillDirectory &;
    auto skills() const -> SkillDirectory const &;

    auto target() const -> OptRef<Actor>;

    bool isAlive()          const;
    bool isDead()           const;
    bool isInCombatStance() const;

    auto currentAction() -> OptRef<Action>;
    auto nextAction()    -> OptRef<Action>;

    auto effects()       -> std::list<std::unique_ptr<Effect>>       &;
    auto effects() const -> std::list<std::unique_ptr<Effect>> const &;

    auto attackerDamageAmounts() const -> DamageDealtTable const &;

public:
    virtual bool isAttackable() const = 0;

public:
    void setName(std::wstring name);
    void setTitle(std::wstring title);
    void setPosition(Position const & position);
    void setPosition(s32 x, s32 y, s32 z);
    void setPosX(s32);
    void setPosY(s32);
    void setPosZ(s32);
    void setOrientation(s32);
    void setTeam(Team team);
    void setTarget(OptRef<Actor>);

    template<typename A, typename... Args> requires std::is_base_of_v<Action, A>
    void doNext(Args &&... args) { doNext(std::make_unique<A>(*this, std::forward<Args>(args)...)); }
    void cancelAction();

    void takeDamage(OptRef<Actor> emitter, double amount);
    void heal(Actor const & emitter, double amount);
    void die();
    void revive();

    template<typename T, typename... Args> requires std::is_base_of_v<Effect, T>
    void addEffect(Args &&... args) { addEffect(std::make_unique<T>(std::forward<Args>(args)...)); }

private:
    void doNext(std::unique_ptr<Action>);
    void addEffect(std::unique_ptr<Effect>);

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
