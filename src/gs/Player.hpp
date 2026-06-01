/// @author    Chnossos
/// @date      Created on 2026-02-24

#pragma once

// Project includes
#include <common/Pimpl.hpp>
#include <gs/Typedefs.hpp>

// Third-party includes
#include <boost/asio/ip/tcp.hpp>

class Character;
namespace Network { class Connection; }

class Player
{
public:
    explicit Player(boost::asio::ip::tcp::socket && socket);
    ~Player();

public:
    auto id()        const -> u32;
    auto accountId() const -> AccountId;

    auto connection()       -> Network::Connection &;
    auto connection() const -> Network::Connection const &;

    auto currentCharacter() -> OptRef<Character>;

    auto accountName()   const -> std::wstring_view;
    auto playOk1()       const -> u32;

public:
    void setAccountId(AccountId);
    void setAccountName(std::wstring);
    void setPlayOk1(u32);

    void setCurrentCharacter(Character &);
    void unsetCurrentCharacter();

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
