/// @author    Chnossos
/// @date      Created on 2026-04-11

#pragma once

// Project includes
#include <common/Typedefs.hpp>

// Third-party
#include <boost/asio/ip/tcp.hpp>

namespace Network { class Packet; }

struct Connection
{
    u32 sessionId;
    boost::asio::ip::tcp::socket socket;

    std::vector<byte> readBuffer;

    std::string userName;
    std::string password;

    std::function<void()>                          onSocketClosed;
    std::function<void(std::vector<byte> const &)> onPacketRead;

    explicit Connection(boost::asio::ip::tcp::socket socket);

    bool operator==(Connection const & other) const;

    void close();

    void asyncReadNextPacket();

    void send(Network::Packet & p, bool encryptPacket = true,
              std::source_location src = std::source_location::current());

private:
    void onSizeRead(boost::system::error_code const & ec);

    void onBodyRead(boost::system::error_code const & ec);

    /// @returns @c true if the async operation can be executed again, else @c false
    bool handleError(boost::system::error_code const & ec, std::string_view src) const;
};

