/// @author    Chnossos
/// @date      Created on 2026-03-22

#pragma once

// Project includes
#include "../../Typedefs.hpp"
#include "../../game/constants/HitStatus.hpp"
#include "../../game/constants/ItemGrade.hpp"

#include <common/Pimpl.hpp>
#include <common/network/Serialization.hpp>

class Actor;

/// Infos about a physical hit.
class Hit
{
    DECLARE_PACKET_SERIALIZATION_OPERATOR(Hit);

public:
    Hit(Actor const & target, u32 damage, bool isCritical = false,
        std::optional<ItemGrade> soulShotUsed = {}, HitStatus status = HitStatus::Landed);

    Hit(Hit &&) noexcept;
    Hit & operator=(Hit &&) noexcept;
    ~Hit();

public:
    auto targetId() const -> u32;
    auto damage() const -> u32;
    auto flags() const -> u8;

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
