/// @author    Chnossos
/// @date      Created on 2026-02-16

#include "SocketListener.hpp"

// Project includes
// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>

// Third-party includes
#include <spdlog/spdlog.h>

// C++ includes
#include <functional>
#include <unordered_map>

namespace ip = boost::asio::ip; // NOLINT(*-identifier-naming)

using boost::system::error_code;

using SL = Network::SocketListener;
using TcpAcceptor = ip::tcp::acceptor;

struct SL::Impl
{
    boost::asio::io_context &               ioContext;
    std::unordered_map<u16, TcpAcceptor>    acceptors;
    std::unordered_map<u16, AcceptCallback> callbacks;

    explicit Impl(boost::asio::io_context & ioc)
        : ioContext(ioc)
    {}
    ~Impl() = default;

    bool listen(ip::address const & addr, u16 port, AcceptCallback cb);
    void onAccept(u16 port, error_code const & ec, ip::tcp::socket socket);
    bool handleError(u16 port, error_code const & ec) const;
};

template class Pimpl<Network::SocketListener::Impl>;

bool SL::Impl::listen(ip::address const & addr, u16 const port, AcceptCallback cb)
{
    ip::tcp::acceptor acceptor{ioContext};
    try
    {
        ip::tcp::endpoint const endpoint{addr, port};
        acceptor.open(endpoint.protocol());
        acceptor.set_option(ip::tcp::acceptor::reuse_address{true});
        acceptor.bind(endpoint);
        acceptor.listen();
    }
    catch (error_code const & ec)
    {
        return handleError(port, ec);
    }

    using namespace std::placeholders;
    acceptor.async_accept(std::bind(&Impl::onAccept, this, port, _1, _2));

    acceptors.emplace(port, std::move(acceptor));
    callbacks.emplace(port, std::move(cb));
    return true;
}

void SL::Impl::onAccept(u16 port, error_code const & ec, ip::tcp::socket socket)
{
    if (!handleError(port, ec)) return;

    using namespace std::placeholders;
    std::invoke(callbacks.at(port), std::move(socket));
    acceptors.at(port).async_accept(std::bind(&Impl::onAccept, this, port, _1, _2));
}

bool SL::Impl::handleError(u16 port, error_code const & ec) const
{
    switch (auto const code = ec.default_error_condition().value())
    {
        namespace errc = boost::system::errc; // NOLINT(*-identifier-naming)

        case errc::success:
            return true;

        case errc::operation_canceled:
            SPDLOG_INFO("[SocketListener] [{}] shut down successfully", port);
            break;

        default:
            SPDLOG_ERROR("[SocketListener] [{}] error {}: {}", port, code, ec.message());
            break;
    }
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

SL::SocketListener(boost::asio::io_context & ioc)
    : _impl(ioc)
{}

SL::~SocketListener() = default;

bool SL::listen(std::string_view const host, u16 const port, AcceptCallback cb)
{
    error_code ec;
    auto const addr = ip::make_address(std::string{host}, ec);
    return ec ? _impl->handleError(port, ec) : _impl->listen(addr, port, std::move(cb));
}

void SL::shutdown()
{
    SPDLOG_INFO("[SocketListener] Shutting down all ports");

    for (auto & [port, acceptor] : _impl->acceptors)
    {
        error_code ec;
        _impl->handleError(port, acceptor.cancel(ec));
    }

    _impl->acceptors.clear();
}
