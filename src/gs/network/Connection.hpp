/// @author    Chnossos
/// @date      Created on 2026-02-22

#pragma once

// Project includes
#include "../Typedefs.hpp"

#include <common/Pimpl.hpp>

// Third-party includes
#include <boost/asio/ip/tcp.hpp>

// C++ includes
#include <span>

namespace Network { class Packet; }
namespace Network { class Connection; }

class Network::Connection
{
public:
    using ConnectionClosedHandler = std::function<void()>;
    using PacketReceivedHandler   = std::function<void(std::span<byte const>)>;

public:
    explicit Connection(boost::asio::ip::tcp::socket && socket);
    ~Connection();

public:
    auto id()            const -> ConnectionId;
    bool isAlive()       const;
    auto readBuffer()    const -> std::span<byte const>;
    auto encryptionKey() const -> std::span<byte const>;

public:
    void asyncReadNextPacket();
    void send(Network::Packet & p, std::source_location const & src = std::source_location::current());
    void send(Network::Packet && p,
              std::source_location const & src = std::source_location::current()) { send(p, src); }
    void close();

    void setOnConnectionClosed(ConnectionClosedHandler);
    void setOnPacketReceivedHandler(PacketReceivedHandler);

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
