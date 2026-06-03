/// @author    Chnossos
/// @date      Created on 2026-03-16

#include "World.hpp"

// Project includes
#include <common/CompileTimeConfig.hpp>
#include <gs/Player.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/actor/Monster.hpp>
#include <gs/game/actor/Npc.hpp>
#include <gs/game/components/ActorAutoRegen.hpp>
#include <gs/game/components/AttackStanceTimer.hpp>
#include <gs/game/components/CharacterSelectionData.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/DeletionTimer.hpp>
#include <gs/game/components/Loot.hpp>
#include <gs/game/components/NpcAppearance.hpp>
#include <gs/game/components/NpcStatus.hpp>
#include <gs/game/components/PlayerAppearance.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/constants/Profession.hpp>
#include <gs/game/constants/Sex.hpp>
#include <gs/game/constants/SystemMessageId.hpp>
#include <gs/game/directories/CharacterTemplateDirectory.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/ecs/System.hpp>
#include <gs/game/lobby/CharacterCreationParameters.hpp>
#include <gs/game/systems/ActorAttackStanceTimerSystem.hpp>
#include <gs/game/systems/ActorAutoRegenSystem.hpp>
#include <gs/game/systems/ActorDeletionTimerSystem.hpp>
#include <gs/game/systems/ActorEffectSystem.hpp>
#include <gs/game/systems/ActorStatsUpdateSystem.hpp>
#include <gs/network/Connection.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/status/ActorDiePacket.hpp>
#include <gs/network/packets/server/status/ActorRevivePacket.hpp>
#include <gs/network/packets/server/status/EffectListPacket.hpp>
#include <gs/network/packets/server/target/TargetClearPacket.hpp>
#include <gs/network/packets/server/world/GameObjectDeletePacket.hpp>
#include <gs/orm/Characters.hpp>
#include <gs/orm/Lobby.hpp>
#include <gs/utils/Conversion.hpp>
#include <gs/utils/Maths.hpp>

// Third-party
#include <spdlog/spdlog.h>

// C++ includes
#include <ranges>

namespace SC = Network::Packets::Server; // Server -> Client

using Network::Packet;

static void addGremlin()
{
    static u32 count = 1;

    if (auto const gremlin = World::addNpc(1))
        gremlin->setPosX(gremlin->position().x + (count++ % 2 ? 35 : -35));
}

static void addDummy()
{
    static u32 count = 1;

    auto & d = World::addCharacter();
    d.setPosY(d.position().y + (count++ % 2 ? 35 : -35));
    d.setName(std::format(L"dummy{}", d.id()));
    d.appearance().setStartingProfession(Profession::ElvenMystic);
    d.appearance().setSex(Sex::Female);
    d.appearance().setCollisionHeight(23);
    d.appearance().setCollisionRadius(7.5);
    d.setProfession(d.appearance().startingProfession());

    auto & loot = d.addComponent<Loot>();
    loot.xp = 50;
}

void World::init()
{
    registerSystem<ActorAttackStanceTimerSystem>();
    registerSystem<ActorAutoRegenSystem>();
    registerSystem<ActorDeletionTimerSystem>();
    registerSystem<ActorEffectSystem>();
    // Must be last!
    registerSystem<ActorStatsUpdateSystem>();

    addGremlin();
    addGremlin();

    addDummy();
    addDummy();
}

auto World::actor(GameObjectId const id) -> OptRef<Actor>
{
    auto const it = _actors.find(id);
    return it != _actors.end() ? OptRef(*it->second) : std::nullopt;
}

auto World::character(GameObjectId const id) -> OptRef<Character>
{
    auto const actor = World::actor(id);
    return actor && actor->type() == ActorType::Character ? OptRef(static_cast<Character &>(*actor)) : std::nullopt;
}

auto World::monster(GameObjectId const id) -> OptRef<Monster>
{
    auto const actor = World::actor(id);
    return actor && actor->type() == ActorType::Monster ? OptRef(static_cast<Monster &>(*actor)) : std::nullopt;
}

auto World::npc(GameObjectId const id) -> OptRef<Npc>
{
    auto const actor = World::actor(id);
    return actor && actor->type() == ActorType::Npc ? OptRef(static_cast<Npc &>(*actor)) : std::nullopt;
}

void World::update(ClockDuration const elapsed)
{
    using namespace std::chrono;

    for (auto const & a : _actors | std::views::values)
    {
        try
        {
            if (auto const action = a->currentAction())
                action->update(elapsed);
        }
        catch (Core::Exception const & e)
        {
            SPDLOG_ERROR("Failed to update action for character '{}':\n{}", a->id(), Core::formatExceptionStack(e));
        }
        catch (std::exception const & e)
        {
            SPDLOG_ERROR("Failed to update action for character '{}':\n{}", a->id(), Core::formatExceptionStack(e));
        }
    }

    for (auto const & system : _systems)
    {
        for (auto const & a : _actors | std::views::values)
            system->update(elapsed, *a);
    }

    for (Actor & a : _scheduledForDeletion | std::views::values)
        delActor(a);

    _scheduledForDeletion.clear();
}

auto World::createCharacter(Player const & p, CharacterCreationParameters const & params) -> CharacterCreationResult
{
    auto & c = addCharacterPreview(p.accountId());
    c.setName(params.name);
    c.setProfession(params.profession);
    c.addComponent<CharacterSelectionData>().selected = true;

    // FIXME: change values depending on race & class
    c.setPosition(-83968, 244634, -3500); // Talking Island GK

    auto & a = c.appearance();
    a.setStartingProfession(params.profession);
    a.setSex               (params.sex);
    a.setHairStyle         (params.hairStyle);
    a.setHairColor         (params.hairColor);
    a.setFace              (params.face);
    a.setCollisionHeight   (CharacterTemplateDirectory::collisionHeight(a.startingProfession(), a.sex()));
    a.setCollisionRadius   (CharacterTemplateDirectory::collisionRadius(a.startingProfession(), a.sex()));

    Orm::createCharacter(p.accountId(), c);
    return CharacterCreationResult::Success;
}

auto World::getCharacterPreviews(Player const & p) -> std::vector<Ref<Character>>
{
    std::vector<Ref<Character>> result;

    if (!_characterPreviewsIndex.contains(p.accountId())) // no index means first connection since server booted
        result = Orm::loadCharacterPreviews(p.accountId());
    else
    {
        auto const & index = _characterPreviewsIndex[p.accountId()];
        result.reserve(index.size());

        for (auto const id : index)
            result.emplace_back(*_characterPreviews.at(id));
    }

    return result;
}

auto World::addCharacterPreview(AccountId const accountId) -> Character &
{
    L2CPP_B_ASSERT(accountId, "Player account id unknown, cannot create character preview");

    Ref c = addCharacter();
    auto const id = c.get().id();
    _characterPreviewsIndex[accountId].emplace_back(id);
    c = *_characterPreviews.try_emplace(id, static_cast<Character *>(_actors[id].release())).first->second;
    _actors.erase(id);
    return c;
}

auto World::loadCharacterFromPreview(Character const & c) -> Character &
{
    L2CPP_B_ASSERT(_characterPreviews.contains(c.id()), "Character '{}' is not loaded in previews", c.id());

    auto const id    = c.id();
    auto const & ptr = _actors.try_emplace(id, _characterPreviews[id].release()).first->second;
    _characterPreviews.erase(id);

    auto & character = static_cast<Character &>(*ptr);
    Orm::loadCharacter(character);
    return character;
}

void World::moveCharacterBackToPreviews(Character & c)
{
    L2CPP_B_ASSERT(_actors.contains(c.id()), "Character '{}' is not present in the world", c.id());

    c.onEffectListChanged.clear();
    c.onDied                     .clear();
    c.onLeveledUp                .clear();
    c.onRevived                  .clear();

    c.delComponent<ActorAutoRegen>();
    c.delComponent<AttackStanceTimer>();

    if (auto const target = c.target())
    {
        unsubscribeFromTarget(target, c);
        c.setTarget(std::nullopt);
    }

    unsubscribeAllTargetListeners(c);
    broadcastAround(c, SC::GameObjectDeletePacket{c});

    if (c.player)
        c.player->unsetCurrentCharacter();

    Orm::saveCharacter(c);

    auto const id = c.id();
    auto & ptr = _actors.at(id);
    _characterPreviews.try_emplace(id, static_cast<Character *>(ptr.release()));
    _actors.erase(id);
}

void World::delCharacterPreview(AccountId const accountId, size_t const index)
try
{
    L2CPP_B_ASSERT(_characterPreviewsIndex.contains(accountId), "Account have no character previews existing/loaded");
    auto & previewIds = _characterPreviewsIndex.at(accountId);

    L2CPP_B_ASSERT(index < previewIds.size(), "Requested index is out of range");

    auto const & c = *_characterPreviews.at(previewIds[index]);
    Orm::deleteCharacter(c.name());
    _characterPreviews.erase(previewIds[index]);
    previewIds.erase(previewIds.begin() + index, previewIds.end());
}
catch (...)
{
    L2CPP_THROW_NESTED("Failed to delete character preview");
}

auto World::addCharacter(OptRef<Player> p) -> Character &
{
    auto & c = addActor<Character>(std::move(p));
    c.onEffectListChanged += [&c] { send(c, SC::EffectListPacket{c});                                         };
    c.onLeveledUp         += [&c] { send(c, SC::ChatSystemSayPacket{SystemMessageId::YourLevelHasIncreased}); };
    return c;
}

auto World::addNpc(u32 id) -> OptRef<Npc>
{
    OptRef<Npc> npc;

    if (auto const info = NpcDirectory::find(id))
    {
        npc = info->type == ActorType::Npc ? addActor<Npc>(id) : addActor<Monster>(id);
        npc->setName(Utils::toWideString(info->name));

        if (info->title.empty())
            npc->setTitle(std::format(L"Lv. {}", npc->status().level()));
        else
            npc->setTitle(Utils::toWideString(info->title));

        npc->appearance().setCollisionHeight(15);
        npc->appearance().setCollisionRadius(10);

        if (npc->type() == ActorType::Monster)
        {
            auto & loot = npc->addComponent<Loot>();
            loot.xp = 29;
            loot.sp = 2;
        }

        npc->onDied += [&n = *npc]
        {
            scheduleForDeletion(n, 15s);
            if (auto const loot = n.component<Loot>())
                distributeLoot(loot, n.attackerDamageAmounts());
        };
    }

    return npc;
}

void World::scheduleForDeletion(Actor & a, ClockDuration const timeFromNow)
{
    if (timeFromNow > ClockDuration::zero())
    {
        auto & timer = a.getOrAddComponent<DeletionTimer>();
        timer.timeBeforeDeletion = timeFromNow;
        timer.elapsedSinceDeath  = ClockDuration::zero();
    }
    else
        _scheduledForDeletion.try_emplace(a.id(), a);
}

void World::unscheduleForDeletion(Actor & a)
{
    _scheduledForDeletion.erase(a.id());
    a.delComponent<DeletionTimer>();
}

auto World::inGameTime() -> std::chrono::minutes
{
    if constexpr (Config::isDebugMode)
        return 60min * 10; // 10am because we want sunlight to see what we're doing
    else
    {
        constexpr u32                  inGameTimeAcceleration = 6; // means 1 in-game day equals 4 hours IRL
        constexpr std::chrono::minutes irlDayInMinutes{60min * 24};
        constexpr std::chrono::minutes inGameDayInMinutes{irlDayInMinutes / inGameTimeAcceleration};

        auto const now                   = std::chrono::system_clock::now(); // Server time is always UTC time
        auto const irlTimeOfDay          = now - std::chrono::floor<std::chrono::days>(now);
        auto const irlTimeOfDayInMinutes = std::chrono::floor<std::chrono::minutes>(irlTimeOfDay);
        return irlTimeOfDayInMinutes % inGameDayInMinutes * inGameTimeAcceleration;
    }
}

auto World::subscribeToTarget(GameObjectId const targetId, Actor const & listener) -> OptRef<Actor>
{
    OptRef<Actor> target;

    if (listener.type() == ActorType::Character)
    {
        if (target = actor(targetId); target)
            subscribeToTarget(target, listener);
    }

    return target;
}

void World::subscribeToTarget(Actor const & target, Actor const & listener)
{
    if (listener.type() != ActorType::Character || listener == target) // do not subscribe to yourself
        return;

    if (auto const currentTarget = listener.target())
        unsubscribeFromTarget(currentTarget, listener);

    _targetSubscribers[target.id()].emplace_back(listener.id());
}

void World::unsubscribeFromTarget(Actor const & target, Actor const & listener)
{
    _targetSubscribers[target.id()].remove(listener.id());
}

void World::unsubscribeAllTargetListeners(Actor const & target)
{
    using namespace std::views;
    for (auto const c : _targetSubscribers[target.id()] | transform([] (auto const id) { return character(id); }))
    {
        if (c)
        {
            c->setTarget(std::nullopt);
            send(c, SC::TargetClearPacket{c});
        }
    }

    _targetSubscribers[target.id()].clear();
}

void World::distributeLoot(Loot const & loot, DamageDealtTable const & attackerDamageAmounts)
{
    std::map<StatValue, Ref<Character>> participants;

    for (auto const [id, dmg] : attackerDamageAmounts)
    {
        if (auto const c = character(id))
            participants.try_emplace(dmg, c);
    }

    auto & c = participants.rbegin()->second.get(); // For now, select the one who dealt the most damage

    auto const oldLevel = c.status().level();
    auto const oldXp    = c.status().xp();
    auto const oldSp    = c.status().sp();

    c.status().addXp(loot.xp);
    c.status().addSp(loot.sp);

    auto const newLevel  = c.status().level();
    bool const leveledUp = newLevel > oldLevel;
    auto const newXp     = c.status().xp();
    auto const newSp     = c.status().sp();

    if (leveledUp)
        c.status().setLevel(newLevel);

    std::optional<SC::ChatSystemSayPacket> msg;
    /**/ if (newXp > oldXp && newSp > oldSp)
    {
        msg.emplace(SystemMessageId::Earned_1_XpAnd_2_Sp);
        *msg << SysMsgArg::Number{newXp - oldXp} << SysMsgArg::Number{newSp - oldSp};
    }
    else if (newXp > oldXp)
    {
        msg.emplace(SystemMessageId::Earned_1_Xp);
        *msg << SysMsgArg::Number{newXp - oldXp};
    }
    else if (newSp > oldSp)
    {
        msg.emplace(SystemMessageId::Earned_1_Sp);
        *msg << SysMsgArg::Number{newSp - oldSp};
    }

    if (msg)
        send(c, std::move(*msg));

    if (leveledUp)
        fire c.onLeveledUp();
}

void World::forEachActorAround(Actor const & source, std::function<void(Actor &)> const & f)
{
    if (f)
    {
        auto const distancePred = [&] (auto const & a) { return isInBroadcastRange(source, *a); };
        auto const skipEmitter  = [&] (auto const & a) { return *a != source;                   };

        using namespace std::views;
        for (auto & a : _actors | values | filter(distancePred) | filter(skipEmitter))
            f(*a);
    }
}

bool World::isInBroadcastRange(Actor const & source, Actor const & target)
{
    return Utils::Maths::distance(source, target) <= 6000;
}

void World::send(Actor const & to, Packet & packet, std::source_location const & src)
{
    if (to.type() == ActorType::Character)
    {
        if (auto const & c = static_cast<Character const &>(to); c.player)
            c.player->connection().send(packet, src);
    }
}

void World::broadcast(Packet && packet, std::source_location const & src)
{
    for (auto const & a : _actors | std::views::values)
        send(*a, Packet(packet), src);
}

void World::broadcastAround(Actor const & emitter, Packet && packet, bool const includeEmitter,
                            std::source_location const & src)
{
    auto const charIsInRange      = [&] (auto const & a) { return isInBroadcastRange(emitter, *a); };
    auto const emitterIfRequested = [&] (auto const & a) { return *a != emitter || includeEmitter; };

    using namespace std::views;
    for (auto const & a : _actors | values | filter(charIsInRange) | filter(emitterIfRequested))
        send(*a, Packet(packet), src);
}

void World::broadcastToSubscribers(Actor const & emitter, Packet && packet, bool const includeEmitter,
                                   std::source_location const & src)
{
    auto & subs = _targetSubscribers[emitter.id()];

    for (auto it = subs.begin(); it != subs.end(); )
    {
        if (auto const actorIt = _actors.find(*it); actorIt != _actors.end())
        {
            send(*actorIt->second, Packet(packet), src);
            ++it;
        }
        else
            it = subs.erase(it);
    }

    if (includeEmitter)
        send(emitter, Packet(packet), src);
}

auto World::addActor(std::unique_ptr<Actor> actor) -> Actor &
{
    auto const id = actor->id();
    auto & a = *_actors.try_emplace(id, std::move(actor)).first->second;

    a.onDied    += [&a] { broadcastAround(a, SC::ActorDiePacket{a}, true); };
    a.onRevived += [&a]
    {
        unscheduleForDeletion(a);
        broadcastAround(a, SC::ActorRevivePacket{a}, true);
    };

    return a;
}

// PRIVATE -------------------------------------------------------------------------------------------------------------

void World::delActor(Actor & a)
{
    if (a.type() == ActorType::Character)
    {
        auto & c = static_cast<Character &>(a);
        L2CPP_B_ASSERT(!c.player, "Illegal attempt to delete player driven character (objId: '{}')", a.id());
        moveCharacterBackToPreviews(c);
    }
    else
    {
        unsubscribeAllTargetListeners(a);
        broadcastAround(a, SC::GameObjectDeletePacket{a});
        _actors.erase(a.id());
    }
}

std::vector<std::unique_ptr<System>>                         World::_systems;
std::unordered_map<AccountId,    std::vector<GameObjectId>>  World::_characterPreviewsIndex;
std::unordered_map<GameObjectId, std::unique_ptr<Character>> World::_characterPreviews;
std::unordered_map<GameObjectId, std::unique_ptr<Actor>>     World::_actors;
std::unordered_map<GameObjectId, Ref<Actor>>                 World::_scheduledForDeletion;
std::unordered_map<GameObjectId, std::list<GameObjectId>>    World::_targetSubscribers;
