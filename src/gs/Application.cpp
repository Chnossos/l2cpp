/// @author    Chnossos
/// @date      Created on 2026-03-06

#include "Application.hpp"

// Project includes
#include <common/CompileTimeConfig.hpp>
#include <common/core/Exception.hpp>
#include <common/details/Pimpl.hpp>
#include <common/network/Packet.hpp>
#include <common/network/SocketListener.hpp>
#include <common/services/Database.hpp>
#include <common/utils/Misc.hpp>
#include <gs/Player.hpp>
#include <gs/game/World.hpp>
#include <gs/game/directories/CharacterTemplateDirectory.hpp>
#include <gs/game/directories/ItemTemplateDirectory.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/directories/ProfessionDirectory.hpp>
#include <gs/game/directories/SkillTemplateDirectory.hpp>
#include <gs/game/directories/StartingLocationDirectory.hpp>
#include <gs/game/spawn/SpawnManager.hpp>
#include <gs/handlers/PacketHandlers.hpp>
#include <gs/network/Connection.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/client/ClientForceDisconnectPacket.hpp>
#include <gs/orm/Uids.hpp>
#include <gs/utils/Chrono.hpp>

// Third-party includes
#include <boost/asio.hpp>
#include <spdlog/spdlog.h>

// C++ includes
#include <print>

static void hexdump(std::span<byte const> const buffer)
{
    if constexpr (Config::hexdumpPackets)
    {
        if (auto const dump = Utils::hexdump(buffer.data(), buffer.size()); !dump.empty())
            std::println("{}", dump);
    }
}

struct Application::Impl
{
    std::vector<std::string_view> args;
    boost::asio::io_context       ioContext;
    boost::asio::signal_set       signalSet;
    Network::SocketListener        socketListener;
    std::list<Player>             players;

    explicit Impl(std::vector<std::string_view> args_)
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

template class Pimpl<Application::Impl>;

bool Application::Impl::load() const try
{
    SPDLOG_INFO("Initializing database…");
    Database::init({
        // Order is significant
        "sql/professions.sql",
        "sql/character_templates.sql",
        "sql/characters.sql",
        "sql/character_previews.sql",
        "sql/character_professions.sql",
        "sql/character_statuses.sql",
        "sql/character_shortcuts.sql",
        "sql/item_templates.sql",
        "sql/items.sql",
        "sql/starting_items.sql",
        "sql/starting_locations.sql",
        "sql/gs_data.sql",
    });
    Orm::loadUids();
    SPDLOG_INFO("… database initialization done.");

    SPDLOG_INFO("Loading professions…");
    ProfessionDirectory::load();
    SPDLOG_INFO("… registered {:L} professions", ProfessionDirectory::count());

    SPDLOG_INFO("Loading character templates…");
    CharacterTemplateDirectory::load();
    SPDLOG_INFO("… registered {:L} character templates", CharacterTemplateDirectory::count());

    SPDLOG_INFO("Loading starting locations…");
    StartingLocationDirectory::load();
    SPDLOG_INFO("… registered {:L} starting locations", StartingLocationDirectory::count());

    SPDLOG_INFO("Loading item templates…");
    ItemTemplateDirectory::load();
    SPDLOG_INFO("… registered {:L} item templates", ItemTemplateDirectory::size());

    SPDLOG_INFO("Loading skills…");
    SkillTemplateDirectory::load("data/skillname-e.txt", "data/skillgrp.txt");
    SPDLOG_INFO("… registered {:L} unique skill variants", SkillTemplateDirectory::size());

    SPDLOG_INFO("Loading NPCs info…");
    NpcDirectory::load();
    SPDLOG_INFO("… registered {:L} NPC templates ({:L} NPCs; {:L} monsters)",
                NpcDirectory::totalCount(), NpcDirectory::npcCount(), NpcDirectory::monsterCount());

    SPDLOG_INFO("Loading spawn points…");
    sSpawnManager.load();
    SPDLOG_INFO("… registered {:L} single spawn points", sSpawnManager.loadCount());
    SPDLOG_INFO("… spawned {:L} NPCs",                   sSpawnManager.spawnedCount());

    SPDLOG_INFO("Loading World systems…");
    World::init();
    SPDLOG_INFO("… world systems loaded.");
    return true;
}
catch (Core::Exception const & e)
{
    SPDLOG_ERROR("Failed to load application:\n{}", Core::formatExceptionStack(e));
    return false;
}

int Application::Impl::run() try
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

void Application::Impl::shutdown()
{
    SPDLOG_INFO("Beginning shutting down sequence…");

    SPDLOG_INFO("Shutting down SocketListener…");
    socketListener.shutdown();

    SPDLOG_INFO("Kicking players out…");
    for (auto & player : players)
    {
        auto & conn = player.connection();
        conn.send(Network::Packets::Server::ChatSystemSayPacket{SystemMessageId::DisconnectedFromServer});
        conn.send(Network::Packets::Server::ClientForceDisconnectPacket{});
        conn.close();
    }

    boost::asio::post(ioContext, [] { SPDLOG_INFO("Shutting down sequence done."); });
}

void Application::Impl::onSignal(boost::system::error_code const & ec, int)
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

void Application::Impl::onSocketAccepted(boost::asio::ip::tcp::socket socket) try
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
            catch (Core::Exception const & e)
            {
                SPDLOG_ERROR("'{}' → handler '{}' failed:\n{}",
                             player.connection().id(), handlerName, Core::formatExceptionStack(e));
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
catch (Core::Exception const & e)
{
    SPDLOG_ERROR("Packet reading failed, disconnecting client:\n{}", Core::formatExceptionStack(e));
}

// ---------------------------------------------------------------------------------------------------------------------

Application::Application(std::vector<std::string_view> args): _impl(std::move(args)) {}
Application::~Application() = default;

bool Application::load() { return _impl->load(); }
int  Application::run()  { return _impl->run();  }
