/// @author    Chnossos
/// @date      Created on 2026-03-03

#include "Character.hpp"

// Project includes
#include <common/core/Exception.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>
#include <gs/game/World.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/PlayerAppearance.hpp>
#include <gs/game/components/SkillDirectory.hpp>
#include <gs/game/inventory/ItemStorage.hpp>
#include <gs/game/ui/ShortcutBar.hpp>
#include <gs/network/packets/server/ui/UiConfirmationModalShowPacket.hpp>

enum class ConfirmationModalSystemMessageId : u32
{
    ResurrectionProposal = 1510,
};

struct Character::Impl
{
    bool fullyLoaded = false;

    Profession profession = Profession::HumanFighter;

    ItemStorage inventory;
    ShortcutBar shortcutBar;

    std::optional<ConfirmationModalSystemMessageId> confirmationModalMessageId;
};

template class Pimpl<Character::Impl>;

// ---------------------------------------------------------------------------------------------------------------------

Character::Character(OptRef<Player> p)
    : Actor(ActorType::Character)
    , player(std::move(p))
{
    auto & appearance = addComponent<PlayerAppearance>();
    appearance.setCollisionHeight(23);
    appearance.setCollisionRadius(9);

    addComponent<CharacterStatus>();

    if (accessLevel > 0)
        skills().learn(7029, 4); // Super Haste
}

Character::Character(Character &&) noexcept = default;
Character & Character::operator=(Character &&) noexcept = default;
Character::~Character() = default;

auto Character::isFullyLoaded() const -> bool       { return _impl->fullyLoaded; }
auto Character::profession()    const -> Profession { return _impl->profession;  }

auto Character::status()       -> CharacterStatus       & { return component<CharacterStatus>(); }
auto Character::status() const -> CharacterStatus const & { return component<CharacterStatus>(); }

auto Character::appearance()       -> PlayerAppearance       & { return component<PlayerAppearance>(); }
auto Character::appearance() const -> PlayerAppearance const & { return component<PlayerAppearance>(); }

auto Character::inventory()       -> ItemStorage       & { return _impl->inventory; }
auto Character::inventory() const -> ItemStorage const & { return _impl->inventory; }

auto Character::shortcutBar()       -> ShortcutBar       & { return _impl->shortcutBar; }
auto Character::shortcutBar() const -> ShortcutBar const & { return _impl->shortcutBar; }

void Character::setIsFullyLoaded(bool const isFullyLoaded) { _impl->fullyLoaded = isFullyLoaded; }
void Character::setProfession(Profession const profession) { _impl->profession  = profession;    }

void Character::offerResurrection(Actor const & emitter)
{
    if (isAlive())
        return;

    if (!player)
        return revive();

    using enum ConfirmationModalSystemMessageId;

    Network::Packets::Server::UiConfirmationModalShowPacket p(std::to_underlying(ResurrectionProposal));
    p << SysMsgArg::Text(emitter.name());
    p << SysMsgArg::Text(name());

    World::send(*this, std::move(p));

    _impl->confirmationModalMessageId = ResurrectionProposal;
}

void Character::answerConfirmationModal(u32 const systemMessageId, bool const accepted)
{
    L2CPP_B_ASSERT(_impl->confirmationModalMessageId, "[{}] wasn't expecting any modal answer", id());
    L2CPP_B_ASSERT(std::to_underlying(*_impl->confirmationModalMessageId) == systemMessageId,
                   "[{}] wasn't expecting a confirmation modal answer to system message '{}' (expected id: {})",
                   id(), systemMessageId, std::to_underlying(*_impl->confirmationModalMessageId));

    switch (*_impl->confirmationModalMessageId)
    {
        using enum ConfirmationModalSystemMessageId;

        case ResurrectionProposal:
        {
            if (accepted)
                revive();

            break;
        }

        default:
            L2CPP_THROW("[{}] Unhandled confirmation modal answer for message id '{}'",
                        id(), std::to_underlying(*_impl->confirmationModalMessageId));
    }
}
