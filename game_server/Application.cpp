/// @author    Chnossos
/// @date      Created on 2026-03-06

#include "Application.hpp"

// Project includes
#include "Player.hpp"
#include "game/World.hpp"
#include "game/actor/NpcDirectory.hpp"
#include "game/inventory/ItemTemplateDirectory.hpp"
#include "game/skill/SkillTemplateDirectory.hpp"
#include "handlers/PacketHandlers.hpp"
#include "network/Connection.hpp"
#include "network/packets/server/chat/ChatSystemSayPacket.hpp"
#include "network/packets/server/client/ClientForceDisconnectPacket.hpp"
#include "orm/Uids.hpp"
#include "utils/Chrono.hpp"

#include <l2cpp/CompileTimeConfig.hpp>
#include <l2cpp/Exception.hpp>
#include <l2cpp/Misc.hpp>
#include <l2cpp/details/Pimpl.hpp>
#include <l2cpp/network/Packet.hpp>
#include <l2cpp/network/SocketListener.hpp>
#include <l2cpp/services/Database.hpp>

// Third-party includes
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

// C++ includes
#include <print>

static void hexdump(std::span<byte const> const buffer)
{
    if constexpr (Config::hexdumpPackets)
    {
        if (auto const dump = l2cpp::hexdump(buffer.data(), buffer.size()); !dump.empty())
            std::println("{}", dump);
    }
}

struct Application::ApplicationImpl
{
    std::vector<std::string_view>  args;
    boost::asio::io_context        ioContext;
    boost::asio::signal_set        signalSet;
    l2cpp::Network::SocketListener socketListener;
    std::list<Player>              players;

    explicit ApplicationImpl(std::vector<std::string_view> args_)
        : args(std::move(args_))
        , signalSet(ioContext, SIGINT)
        , socketListener(ioContext)
    {}

    bool load() const;
    int  run();
    void shutdown();

private:
    void onSignal(boost::system::error_code const & ec, int signal);
    void onSocketAccepted(boost::asio::ip::tcp::socket socket);
};

template class Pimpl<Application::ApplicationImpl>;

bool Application::ApplicationImpl::load() const try
{
    SPDLOG_INFO("Initializing database…");
    Database::init({
        // Order is significant
        "sql/characters.sql",
        "sql/character_previews.sql",
        "sql/character_professions.sql",
        "sql/character_shortcuts.sql",
        "sql/item_templates.sql",
        "sql/items.sql",
        "sql/starting_items.sql",
        "sql/gs_data.sql",
    });
    Orm::loadUids();
    SPDLOG_INFO("Database initialization done.");

    SPDLOG_INFO("Loading item templates…");
    ItemTemplateDirectory::load();
    SPDLOG_INFO("Registered {:L} item templates", ItemTemplateDirectory::size());

    SPDLOG_INFO("Loading skills…");
    SkillTemplateDirectory::load("data/skillname-e.txt", "data/skillgrp.txt");
    SPDLOG_INFO("Registered {:L} unique skill variants", SkillTemplateDirectory::size());

    SPDLOG_INFO("Loading NPCs info…");
    NpcDirectory::load("data/npcname-e.txt", "data/npcgrp.txt");
    SPDLOG_INFO("Registered {:L} NPC templates ({:L} NPCs; {:L} monsters)",
                NpcDirectory::totalCount(), NpcDirectory::npcCount(), NpcDirectory::monsterCount());

    SPDLOG_INFO("Loading World systems…");
    World::init();
    SPDLOG_INFO("World systems loaded.");
    return true;
}
catch (l2cpp::Exception const & e)
{
    SPDLOG_ERROR("Failed to load application:\n{}", l2cpp::formatExceptionStack(e));
    return false;
}

int Application::ApplicationImpl::run() try
{
    signalSet.async_wait([this] (auto const & ec, int s) { onSignal(ec, s); });

    constexpr auto ip   = "127.0.0.1";
    constexpr auto port = 7777;

    auto socketAcceptedCb = [this] (auto socket) { onSocketAccepted(std::move(socket)); };
    L2CPP_B_ASSERT(socketListener.listen(ip, port, socketAcceptedCb), "Failed to listen on {}:{}", ip, port);
    SPDLOG_INFO("Listening for clients on {}:{}", ip, port);

    SPDLOG_INFO("Server running. Input CTRL+C to initiate shutdown…");
    Utils::Chrono::Clock loopClock, worldClock, ioClock;
    while (!ioContext.stopped())
    {
        loopClock.restart();

        ioClock.restart();
        if (auto const count = ioContext.poll(); count)
        {
            SPDLOG_TRACE("{} io handlers took {:%Q%q} to execute", count,
                std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(ioClock.restart()));
        }

        World::update(worldClock.restart());

        std::this_thread::sleep_for(100ms - loopClock.restart());
    }

    SPDLOG_INFO("Goodbye.");
    return EXIT_SUCCESS;
}
catch (...)
{
    SPDLOG_CRITICAL("Unhandled exception caught! Performing emergency shutdown…");
    signalSet.cancel();
    shutdown();
    ioContext.run();
    throw;
}

void Application::ApplicationImpl::shutdown()
{
    SPDLOG_INFO("Beginning shutting down sequence…");

    SPDLOG_INFO("Shutting down SocketListener…");
    socketListener.shutdown();

    SPDLOG_INFO("Kicking players out…");
    for (auto & player : players)
    {
        auto & conn = player.connection();
        conn.send(Network::Packet::Server::ChatSystemSayPacket{SystemMessageId::DisconnectedFromServer});
        conn.send(Network::Packet::Server::ClientForceDisconnectPacket{});
        conn.close();
    }

    boost::asio::post(ioContext, [] { SPDLOG_INFO("Shutting down sequence done."); });
}

void Application::ApplicationImpl::onSignal(boost::system::error_code const & ec, int)
{
    switch (ec.default_error_condition().value())
    {
        case boost::system::errc::success:
            SPDLOG_INFO("SIGINT received");
            shutdown();
            break;

        case boost::system::errc::operation_canceled:
            SPDLOG_INFO("SIGINT listener canceled");
            break;

        default:
            SPDLOG_ERROR("onSignal error {}: {}", ec.default_error_condition().value(), ec.message());
            break;
    }
}

void Application::ApplicationImpl::onSocketAccepted(boost::asio::ip::tcp::socket socket) try
{
    auto & player = players.emplace_back(std::move(socket));
    auto & conn   = player.connection();

    auto onConnectionClosed = [this, &player]
    {
        if (auto const c = player.currentCharacter())
            World::moveCharacterBackToPreviews(c);

        players.remove_if([&player] (Player const & p) { return p.accountName() == player.accountName(); });
    };

    auto onPacketReceived = [&player] (std::span<byte const> const buffer)
    {
        auto const size = *reinterpret_cast<PacketHeader const *>(buffer.data());

        PacketOpCode opCode;
        if (buffer[sizeof(size)] == 0xd0)
            opCode = *reinterpret_cast<PacketOpCode const *>(buffer.data() + sizeof(size));
        else
            opCode = buffer[sizeof(size)];

        auto const body = buffer.subspan(sizeof(size) + (opCode > 0xff ? 2 : 1));

        if (auto const it = gPacketHandlers.find(opCode); it != gPacketHandlers.end())
        {
            auto const & [handler, handlerName] = it->second;
            if (opCode > 0xff)
                SPDLOG_INFO("'{}' → 0x{:04x} ({:4} bytes) ({})", player.connection().id(), opCode, size, handlerName);
            else
                SPDLOG_INFO("'{}' → 0x{:02x}   ({:4} bytes) ({})", player.connection().id(), opCode, size, handlerName);

            hexdump(body);

            try { (*handler)(player); }
            catch (l2cpp::Exception const & e)
            {
                SPDLOG_ERROR("'{}' → handler '{}' failed:\n{}",
                             player.connection().id(), handlerName, l2cpp::formatExceptionStack(e));
            }
        }
        else
        {
            if (opCode > 0xff)
                SPDLOG_WARN("'{}' → 0x{:04x} ({:4} bytes) (?)", player.connection().id(), opCode, size);
            else
                SPDLOG_WARN("'{}' → 0x{:02x}   ({:4} bytes) (?)", player.connection().id(), opCode, size);

            hexdump(body);
        }

        if (player.connection().isAlive())
            player.connection().asyncReadNextPacket();
    };

    conn.setOnConnectionClosed([this, onConnectionClosed] {
        boost::asio::post(ioContext, onConnectionClosed);
    });

    conn.setOnPacketReceivedHandler([this, onPacketReceived] (std::span<byte const> buffer) {
        boost::asio::post(ioContext, std::bind(onPacketReceived, buffer));
    });

    conn.asyncReadNextPacket();
}
catch (l2cpp::Exception const & e)
{
    SPDLOG_ERROR("Packet reading failed, disconnecting client:\n{}", l2cpp::formatExceptionStack(e));
}

// ---------------------------------------------------------------------------------------------------------------------

Application::Application(std::vector<std::string_view> args): _impl(std::move(args)) {}
Application::~Application() = default;

bool Application::load() { return _impl->load(); }
int  Application::run()  { return _impl->run();  }
