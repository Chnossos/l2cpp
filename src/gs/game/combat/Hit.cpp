/// @author    Chnossos
/// @date      Created on 2026-03-22

#include "Hit.hpp"

// Project includes
#include "../actor/Actor.hpp"

#include <l2cpp/details/Pimpl.hpp>
#include <l2cpp/network/Packet.hpp>

struct Hit::Impl
{
    u32 targetId;
    u32 damage;
    bool isCritical;
    std::optional<ItemGrade> soulShotUsed;
    HitStatus status;
    u8 flags;

    Impl(u32 const targetId_, u32 const damage_, bool const isCritical_,
         std::optional<ItemGrade> soulShotUsed_, HitStatus const status_
    )
        : targetId(targetId_)
        , damage(damage_)
        , isCritical(isCritical_)
        , soulShotUsed(std::move(soulShotUsed_))
        , status(status_)
        , flags(0)
    {

        if (soulShotUsed)
            flags |= 0x10 | std::to_underlying(*soulShotUsed);

        /**/ if (isCritical)                   flags |= 0x20;
        /**/ if (status == HitStatus::Parried) flags |= 0x40;
        else if (status == HitStatus::Dodged)  flags |= 0x80;
    }
};

template class Pimpl<Hit::Impl>;

// ---------------------------------------------------------------------------------------------------------------------

Hit::Hit(Actor const & target, u32 damage, bool isCritical, std::optional<ItemGrade> soulShotUsed, HitStatus status)
    : _impl(target.id(), damage, isCritical, std::move(soulShotUsed), status)
{}

Hit::Hit(Hit &&) noexcept = default;
Hit & Hit::operator=(Hit &&) noexcept = default;
Hit::~Hit() = default;

auto Hit::targetId() const -> u32 { return _impl->targetId; }
auto Hit::damage()   const -> u32 { return _impl->damage;   }
auto Hit::flags()    const -> u8  { return _impl->flags;    }

DEFINE_PACKET_SERIALIZATION_OPERATOR(Hit, hit)
{
    return p << hit.targetId() << hit.damage() << hit.flags();
}
