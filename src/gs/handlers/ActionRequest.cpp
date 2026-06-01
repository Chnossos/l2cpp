/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actions/AttackAction.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/actor/Monster.hpp>
#include <gs/game/components/Position.hpp>
#include <gs/game/components/Stats.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/action/ActionFailedPacket.hpp>
#include <gs/network/packets/server/status/StatsUpdatePacket.hpp>
#include <gs/network/packets/server/target/TargetMonsterSelectPacket.hpp>
#include <gs/network/packets/server/target/TargetSelectPacket.hpp>

DEFINE_PACKET_HANDLER(ActionRequest) try
{
    L2CPP_B_ASSERT(player.currentCharacter(), "Cannot select target, no current character");

    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 targetId;
    Position origin;
    bool disallowMovement;
    reader >> targetId >> origin >> disallowMovement;

    bool success = true;

    auto & c = *player.currentCharacter();
    if (auto const target = World::actor(targetId))
    {
        if (!c.target() || *c.target() != target) // No current target or trying to change target?
        {
            World::subscribeToTarget(target, c);
            c.setTarget(target);

            if (target->type() != ActorType::Monster)
                c.player->connection().send(TargetSelectPacket{c, target});
            else
            {
                c.player->connection().send(TargetMonsterSelectPacket{c, static_cast<Monster const &>(*target)});

                StatsUpdatePacket p(target);
                p.addStat(Stat::MaxHp, target->stats()[StatId::MaxHp]);
                p.addStat(Stat::CurHp, target->stats()[StatId::CurHp]);
                c.player->connection().send(std::move(p));
            }
        }
        else if (*target != c && target->isAttackable()) // second request on target other than self, launch attack!
        {
            c.state = ActorState::Attacking;
            c.doNext<AttackAction>(c.target(), c.stats()[StatId::PAtkSpeed]);
        }
        else
            success = false;
    }
    else
        success = false;

    if (!success)
        player.connection().send(ActionFailedPacket{});
}
catch (...)
{
    player.connection().send(ActionFailedPacket{});
    throw;
}
