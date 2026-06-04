/// @author    Chnossos
/// @date      Created on 2026-03-16

#include "Npc.hpp"

// Project includes
#include <gs/game/components/Stats.hpp>
#include <gs/game/components/NpcAppearance.hpp>
#include <gs/game/components/NpcStatus.hpp>
#include <gs/game/directories/ProfessionDirectory.hpp>

Npc::Npc(u32 const id)
    : Npc(ActorType::Npc, id)
{}

Npc::Npc(ActorType const type, u32 const id)
    : Actor(type)
{
    addComponent<NpcAppearance>().setId(id);
    addComponent<NpcStatus>();

    auto & stats = *component<Stats>();
    auto const profession = ProfessionDirectory::find(Profession::HumanFighter);
    profession->applyBaseStats(*this);
    stats.regenFully();
}

auto Npc::appearance()       -> NpcAppearance       & { return component<NpcAppearance>(); }
auto Npc::appearance() const -> NpcAppearance const & { return component<NpcAppearance>(); }
auto Npc::status()     const -> NpcStatus     const & { return component<NpcStatus>();     }

