/// @author    Chnossos
/// @date      Created on 2026-02-16

#pragma once

// Project includes
#include <common/Pimpl.hpp>
#include <common/Typedefs.hpp>

// Third-party includes
#include <boost/asio/ip/tcp.hpp>

namespace Network { class SocketListener; }

class Network::SocketListener
{
    using AcceptCallback = std::function<void(boost::asio::ip::tcp::socket)>;

public:
    explicit SocketListener(boost::asio::io_context &);
    ~SocketListener();

public:
    bool listen(std::string_view host, u16 port, AcceptCallback cb);
    void shutdown();

private:
    struct Impl;
    Pimpl<Impl> _impl;
};
