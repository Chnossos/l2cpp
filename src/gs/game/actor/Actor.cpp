/// @author    Chnossos
/// @date      Created on 2026-03-15

#include "Actor.hpp"

// Project includes
#include "../../Player.hpp"
#include "../../network/Connection.hpp"
#include "../../network/packets/server/chat/ChatSystemSayPacket.hpp"
#include "../../network/packets/server/status/StatsUpdatePacket.hpp"
#include "../World.hpp"
#include "../components/ActorAutoRegen.hpp"
#include "../components/ActorIdentity.hpp"
#include "../components/AttackStanceTimer.hpp"
#include "../components/Gear.hpp"
#include "../components/Position.hpp"
#include "../components/SkillDirectory.hpp"
#include "../components/Stats.hpp"

// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>

namespace SC = Network::Packets::Server; // Server -> Client

struct Actor::Impl
{
    ActorType type;
    Team team = Team::None;

    bool isInCombatStance = false;

    OptRef<Actor> target;
    std::unique_ptr<Action> currentAction, nextAction;

    std::list<std::unique_ptr<Effect>> _effects;

    DamageDealtTable attackerDamageAmounts;
};

template class Pimpl<Actor::Impl>;

// ---------------------------------------------------------------------------------------------------------------------

Actor::Actor(ActorType const type)
{
    _impl->type = type;

    addComponent<ActorIdentity>();
    addComponent<Gear>();
    addComponent<Position>(-83968, 244634, -3500); // Talking Island GK
    // addComponent<Position>(-71338, 258271, -3104); // Human Fighter creation spawn point

    auto & skills = addComponent<SkillDirectory>();
    skills.learn(18,   1); // Hate Aura
    skills.learn(78,   1); // War Cry
    skills.learn(81,   1); // Punch of Doom
    skills.learn(129,  1); // Poison
    skills.learn(1016, 1); // Resurrection
    skills.learn(1027, 1); // Group Heal
    skills.learn(1177, 1); // Wind Strike
    skills.learn(1204, 1); // Wind Walk
    skills.learn(1216, 1); // Self Heal
    skills.learn(1217, 1); // Greater Heal
    skills.learn(1229, 1); // Chant of Life
    skills.learn(1231, 1); // Aura Flare
    skills.learn(1254, 1); // Mass Resurrection
    skills.learn(1256, 1); // Heart of Paagrio
    skills.learn(1295, 1); // Aqua Splash

    auto & stats = addComponent<Stats>();
    stats[StatId::BaseStr]           = 40;
    stats[StatId::BaseDex]           = 30;
    stats[StatId::BaseCon]           = 43;
    stats[StatId::BaseInt]           = 21;
    stats[StatId::BaseWit]           = 11;
    stats[StatId::BaseMen]           = 25;
    stats[StatId::BasePAtk]          = 4;
    stats[StatId::BasePDef]          = 80;
    stats[StatId::BaseMAtk]          = 6;
    stats[StatId::BaseMDef]          = 41;
    stats[StatId::BasePAtkSpeed]     = 300;
    stats[StatId::BaseMAtkSpeed]     = 333;
    stats[StatId::BasePAtkRange]     = 20;
    stats[StatId::BasePAtkRandom]    = 10;
    stats[StatId::BaseAccuracy]      = 10;
    stats[StatId::BaseEvasion]       = 10;
    stats[StatId::BasePCritRate]     = 10;
    stats[StatId::BaseMCritRate]     = 10;
    stats[StatId::BaseRunSpeed]      = 115;
    stats[StatId::BaseWalkSpeed]     = 80;
    stats[StatId::BaseSwimRunSpeed]  = 50;
    stats[StatId::BaseSwimWalkSpeed] = 50;

    stats[StatId::BaseHpRegen] = 2.0;
    stats[StatId::BaseMpRegen] = 0.8;
    stats[StatId::BaseCpRegen] = 1.4;

    stats[StatId::BaseMaxCp] = 500;
    stats[StatId::BaseMaxHp] = 500;
    stats[StatId::BaseMaxMp] = 500;

    stats.compute(*this);

    stats[StatId::CurCp] = stats[StatId::MaxCp];
    stats[StatId::CurHp] = stats[StatId::MaxHp];
    stats[StatId::CurMp] = stats[StatId::MaxMp];
}

Actor::Actor(Actor &&) noexcept = default;
Actor & Actor::operator=(Actor &&) noexcept = default;
Actor::~Actor() = default;

bool Actor::operator==(Actor const & other) const { return id() == other.id(); }

auto Actor::type()     const -> ActorType         { return _impl->type;                       }
auto Actor::name()     const -> std::wstring_view { return component<ActorIdentity>()->name;  }
auto Actor::title()    const -> std::wstring_view { return component<ActorIdentity>()->title; }
auto Actor::position() const -> Position const &  { return component<Position>();             }
auto Actor::team()     const -> Team              { return _impl->team;                       }

auto Actor::stats()       -> Stats       & { return *component<Stats>(); }
auto Actor::stats() const -> Stats const & { return *component<Stats>(); }

auto Actor::gear()       -> Gear       & { return *component<Gear>(); }
auto Actor::gear() const -> Gear const & { return *component<Gear>(); }

auto Actor::skills()       -> SkillDirectory       & { return *component<SkillDirectory>(); }
auto Actor::skills() const -> SkillDirectory const & { return *component<SkillDirectory>(); }

auto Actor::target() const -> OptRef<Actor> { return _impl->target; }

bool Actor::isAlive() const { return stats()[StatId::CurHp] >= 1; }
bool Actor::isDead()  const { return stats()[StatId::CurCp] < 1;  }

bool Actor::isInCombatStance() const { return _impl->isInCombatStance; }

auto Actor::currentAction() -> OptRef<Action>
{
    OptRef<Action> action;

    if (_impl->currentAction && _impl->currentAction->isFinished())
        _impl->currentAction = std::move(_impl->nextAction);

    if (_impl->currentAction)
        action = *_impl->currentAction;

    return action;
}

auto Actor::nextAction() -> OptRef<Action> { return _impl->nextAction ? OptRef(*_impl->nextAction) : std::nullopt; }

auto Actor::effects()       -> std::list<std::unique_ptr<Effect>>       & { return _impl->_effects; }
auto Actor::effects() const -> std::list<std::unique_ptr<Effect>> const & { return _impl->_effects; }

auto Actor::attackerDamageAmounts() const -> DamageDealtTable const & {
    return _impl->attackerDamageAmounts;
}

// SETTERS -------------------------------------------------------------------------------------------------------------

void Actor::setName (std::wstring name)  { component<ActorIdentity>()->name  = std::move(name);  }
void Actor::setTitle(std::wstring title) { component<ActorIdentity>()->title = std::move(title); }

void Actor::setPosition(Position const & position)             { *component<Position>() = position; }
void Actor::setPosition(s32 const x, s32 const y, s32 const z) { setPosition(Position{x, y, z});    }

void Actor::setPosX       (s32 const x) { component<Position>()->x           = x; }
void Actor::setPosY       (s32 const y) { component<Position>()->y           = y; }
void Actor::setPosZ       (s32 const z) { component<Position>()->z           = z; }
void Actor::setOrientation(s32 const o) { component<Position>()->orientation = o; }

void Actor::setTeam(Team const team) { _impl->team = team; }

void Actor::setTarget(OptRef<Actor> actor) { _impl->target = std::move(actor); }

void Actor::cancelAction()
{
    if (auto const action = std::move(_impl->currentAction))
        action->cancel();

    _impl->nextAction.reset();
}

void Actor::takeDamage(OptRef<Actor> emitter, double const amount)
{
    if (!isAlive() || amount <= 0)
        return;

    auto & stats = *component<Stats>();
    auto & curHp = stats[StatId::CurHp];

    if ((curHp -= amount) < 1) // almost zero HPs is considered zero HPs
        curHp = 0;

    if (emitter)
    {
        _impl->attackerDamageAmounts[emitter->id()] += amount;

        SC::ChatSystemSayPacket msg1{SystemMessageId::_1_HitYouFor_2_Damage};
        msg1.appendName(emitter);
        msg1.appendArg(SysMsgArg::Number{static_cast<u32>(amount)});
        World::send(*this, std::move(msg1));

        SC::ChatSystemSayPacket msg2{SystemMessageId::YouHitFor_1_Damage};
        msg2.appendArg(SysMsgArg::Number{static_cast<u32>(amount)});
        World::send(emitter, std::move(msg2));
    }
}

void Actor::heal(Actor const & emitter, double const amount)
{
    auto       & stats = *component<Stats>();
    auto       & curHp = stats[StatId::CurHp];
    auto const   maxHp = stats[StatId::MaxHp];

    auto const recovered = curHp + amount > maxHp ? maxHp - curHp : amount;

    if ((curHp += amount) > maxHp)
        curHp = maxHp;

    if (recovered > 0) // Do not send message if no HP actually recovered
    {
        SC::ChatSystemSayPacket msg1{SystemMessageId::_1_HpHaveBeenRestored};
        msg1.appendArg(SysMsgArg::Number{static_cast<u32>(recovered)});
        World::send(*this, std::move(msg1));

        if (emitter != *this)
        {
            SC::ChatSystemSayPacket msg2{SystemMessageId::_2sHpHasBeenRestoredBy_1};
            msg2.appendArg(SysMsgArg::Text{emitter.name()});
            msg2.appendArg(SysMsgArg::Text{name()});
            World::send(emitter, std::move(msg2));
        }
    }
}

void Actor::die()
{
    _impl->_effects.clear();
    fire onEffectListChanged();

    delComponent<ActorAutoRegen>();
    delComponent<AttackStanceTimer>();

    fire onDied();
}

void Actor::revive()
{
    if (isAlive())
        return;

    addComponent<ActorAutoRegen>();

    fire onRevived();

    auto & stats = *component<Stats>();
    stats[StatId::CurHp] = stats[StatId::MaxHp] * 0.65;

    SC::StatsUpdatePacket p(*this);
    p.addStat(Stat::CurHp, stats[StatId::CurHp]);
    World::broadcastToSubscribers(*this, std::move(p), true);
}

void Actor::doNext(std::unique_ptr<Action> action)
{
    if (!action)
        cancelAction();
    else if (!_impl->currentAction)
        _impl->currentAction = std::move(action);
    else if (_impl->currentAction->canBeInterruptedByAnotherAction())
    {
        cancelAction();
        _impl->currentAction = std::move(action);
    }
    else
        _impl->nextAction = std::move(action);
}

void Actor::addEffect(std::unique_ptr<Effect> e) { _impl->_effects.emplace_back(std::move(e)); }
