/// @author    Chnossos
/// @date      Created on 2026-02-24

#include "Player.hpp"

// Project includes
#include <common/core/Exception.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>
#include <gs/game/World.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/network/Connection.hpp>

struct Player::Impl
{
    Network::Connection conn;
    AccountId           id;
    std::wstring        accountName;
    u32                 playOk1;

    OptRef<Character> currentCharacter;

    explicit Impl(boost::asio::ip::tcp::socket && socket);
};

template class Pimpl<Player::Impl>;

Player::Impl::Impl(boost::asio::ip::tcp::socket && socket)
    : conn(std::move(socket))
    , id()
    , playOk1()
{}

// ---------------------------------------------------------------------------------------------------------------------

Player::Player(boost::asio::ip::tcp::socket && socket)
    : _impl(std::move(socket))
{}

Player::~Player() = default;

auto Player::id()        const -> AccountId { return _impl->conn.id(); }
auto Player::accountId() const -> AccountId { return _impl->id; }

auto Player::connection()       -> Network::Connection       & { return _impl->conn; }
auto Player::connection() const -> Network::Connection const & { return _impl->conn; }

auto Player::currentCharacter() -> OptRef<Character> { return _impl->currentCharacter;  }

auto Player::accountName() const -> std::wstring_view { return _impl->accountName; }
auto Player::playOk1()     const -> u32               { return _impl->playOk1;     }

void Player::setAccountId(AccountId const id)      { _impl->id = id;                           }
void Player::setAccountName(std::wstring userName) { _impl->accountName = std::move(userName); }
void Player::setPlayOk1(u32 const playOk1)         { _impl->playOk1 = playOk1;                 }

void Player::setCurrentCharacter(Character & c)
{
    _impl->currentCharacter = c;
    c.player = *this;
}

void Player::unsetCurrentCharacter()
{
    if (_impl->currentCharacter)
    {
        _impl->currentCharacter->player.reset();
        _impl->currentCharacter.reset();
    }
}
