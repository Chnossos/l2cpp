/// @author    Chnossos
/// @date      Created on 2026-02-22

#include "Connection.hpp"

// Project includes
#include <common/CompileTimeConfig.hpp>
#include <common/core/Exception.hpp>
// ReSharper disable once CppUnusedIncludeDirective
#include <common/details/Pimpl.hpp>
#include <common/network/Packet.hpp>
#include <common/utils/Misc.hpp>

// Third-party includes
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <spdlog/spdlog.h>

// C++ includes
#include <algorithm>
#include <filesystem>
#include <print>

using Network::Connection;

using EncryptionKey = std::array<byte, sizeof(u64)>;
constexpr EncryptionKey gEncryptionKey{{0x94, 0x35, 0x00, 0x00, 0xa1, 0x6c, 0x54, 0x87}};

struct Connection::Impl
{
    ConnectionId                 id;
    boost::asio::ip::tcp::socket socket;
    std::vector<byte>            readBuffer;
    std::optional<EncryptionKey> encryptionKey, decryptionKey;
    ConnectionClosedHandler      closedHandler;
    PacketReceivedHandler        packetHandler;

    explicit Impl(boost::asio::ip::tcp::socket && socket);

    void encrypt(std::span<byte>, EncryptionKey &) const;
    void decrypt(std::span<byte>, EncryptionKey &) const;
    void close();

    void onSizeRead(boost::system::error_code const &);
    void onBodyRead(boost::system::error_code const &);
    bool handleReadError(boost::system::error_code const &, std::string_view);
};

template class Pimpl<Connection::Impl>;

Connection::Impl::Impl(boost::asio::ip::tcp::socket && socket)
    : socket(std::move(socket))
{
    static decltype(id) uid = 0;
    id = ++uid;
    readBuffer.resize(sizeof(PacketHeader));
}

void Connection::Impl::encrypt(std::span<byte> data, EncryptionKey & key) const
{
    decltype(data)::value_type tmp = 0;
    for (size_t i = 0; i < data.size(); tmp = data[i++])
        data[i] ^= key[i & 7] ^ tmp;

    *reinterpret_cast<u32 *>(key.data()) += static_cast<u32>(data.size());
}

void Connection::Impl::decrypt(std::span<byte> data, EncryptionKey & key) const
{
    decltype(data)::value_type tmp1 = 0, tmp2 = 0;
    for (size_t i = 0; i < data.size(); ++i)
    {
        tmp1     = data[i];
        data[i] ^= key[i & 7] ^ tmp2;
        tmp2     = tmp1;
    }

    *reinterpret_cast<u32 *>(key.data()) += static_cast<u32>(data.size());
}

void Connection::Impl::close()
{
    if (socket.is_open())
    {
        boost::system::error_code ec;
        if (socket.shutdown(socket.shutdown_both, ec); ec)
            SPDLOG_ERROR("shutdown error '{}' ({}): {}", ec.category().name(), ec.value(), ec.message());

        if (socket.close(ec); ec)
            SPDLOG_ERROR("close error '{}' ({}): {}", ec.category().name(), ec.value(), ec.message());

        if (closedHandler)
            closedHandler();
    }
}

void Connection::Impl::onSizeRead(boost::system::error_code const & ec)
{
    if (ec.value() == boost::asio::error::operation_aborted || !handleReadError(ec, "size"))
        return;

    readBuffer.resize(*reinterpret_cast<PacketHeader const *>(readBuffer.data()));
    auto const payload = std::span(readBuffer).subspan(sizeof(PacketHeader));
    boost::asio::async_read(socket, boost::asio::buffer(payload), [this] (auto const & e, auto) { onBodyRead(e); });
}

void Connection::Impl::onBodyRead(boost::system::error_code const & ec)
{
    if (ec.value() == boost::asio::error::operation_aborted || !handleReadError(ec, "body"))
        return;

    if (decryptionKey) [[likely]]
        decrypt(std::span(readBuffer).subspan(sizeof(PacketHeader)), *decryptionKey);
    else
        decryptionKey.emplace(gEncryptionKey);

    if (packetHandler)
        packetHandler(readBuffer);
}

bool Connection::Impl::handleReadError(boost::system::error_code const & ec, std::string_view source)
{
    switch (auto const code = ec.value())
    {
        case boost::system::errc::success:
            return true;
        case boost::asio::error::connection_reset: // Client closed the connection
        {
            SPDLOG_WARN("Client {} connection reset during packet {} reading: {} ({}:{})",
                        id, source, ec.message(), ec.category().name(), code);
            [[fallthrough]];
        }
        case boost::asio::error::eof:
        {
            SPDLOG_INFO("Client {} disconnected by themselves", id);
            break;
        }
        default:
        {
            SPDLOG_ERROR("Client {} failed to read next packet {}: {} ({}:{})",
                         id, source, ec.message(), ec.category().name(), code);
            break;
        }
    }
    close();
    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

Connection::Connection(boost::asio::ip::tcp::socket && socket)
    : _impl(std::move(socket))
{}

Connection::~Connection() { _impl->close(); }

auto Connection::id()            const -> ConnectionId          { return _impl->id;               }
auto Connection::isAlive()       const -> bool                  { return _impl->socket.is_open(); }
auto Connection::readBuffer()    const -> std::span<byte const> { return _impl->readBuffer;       }
auto Connection::encryptionKey() const -> std::span<byte const> { return gEncryptionKey;          }

void Connection::asyncReadNextPacket()
{
    if (!isAlive())
        return;

    boost::asio::async_read(_impl->socket, boost::asio::buffer(_impl->readBuffer.data(), sizeof(PacketHeader)),
                            [this] (auto const & ec, auto) { _impl->onSizeRead(ec); });
}

void Connection::send(Packet & p, std::source_location const & src)
{
    if (!isAlive())
        return;

    [[maybe_unused]] std::string hexdump;
    if constexpr (Config::hexdumpPackets)
        hexdump = Utils::hexdump(p.body());

    if (_impl->encryptionKey) [[likely]]
        _impl->encrypt(p.body(), *_impl->encryptionKey);
    else
        _impl->encryptionKey.emplace(gEncryptionKey);

    boost::system::error_code ec;
    boost::asio::write(_impl->socket, boost::asio::buffer(p.buffer()), ec);
    if (ec)
        SPDLOG_ERROR("send error {} (category: {}): {}", ec.value(), ec.category().name(), ec.message());
    else
    {
        auto const name = p.name().empty() ? "?" : p.name();
        if (p.opCode() > 0xff)
        {
            SPDLOG_INFO("'{}' ← 0x{:04x} ({:4} bytes) ({}) (from {}:{})", _impl->id, p.opCode(), p.size(),
                       name, std::filesystem::path(src.file_name()).filename().string(), src.line());
        }
        else
        {
            SPDLOG_INFO("'{}' ← 0x{:02x}   ({:4} bytes) ({}) (from {}:{})", _impl->id, p.opCode(), p.size(),
                        name, std::filesystem::path(src.file_name()).filename().string(), src.line());
        }

        if constexpr (Config::hexdumpPackets)
            std::println("{}", hexdump);
    }
}

void Connection::close()                                             { _impl->close();                      }
void Connection::setOnConnectionClosed(ConnectionClosedHandler h)    { _impl->closedHandler = std::move(h); }
void Connection::setOnPacketReceivedHandler(PacketReceivedHandler h) { _impl->packetHandler = std::move(h); }
