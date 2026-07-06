/// @author    Chnossos
/// @date      Created on 2026-02-13

// Project includes
#include <common/Typedefs.hpp>
#include <common/core/Exception.hpp>
#include <common/network/Packet.hpp>
#include <common/network/SocketListener.hpp>
#include <common/services/Database.hpp>
#include <ls/Constants.hpp>
#include <ls/Orm.hpp>
#include <ls/crypto/Blowfish.hpp>
#include <ls/crypto/Checksum.hpp>
#include <ls/crypto/Rsa.hpp>
#include <ls/network/Connection.hpp>
#include <ls/network/OpCodes.hpp>

// Third-party includes
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <openssl/rand.h>
#include <spdlog/spdlog.h>

// C++ includes
#include <list>

using Network::Packet;

static void sendInitPacket(Connection & conn)
{
    Packet p(ServerOpCode::Initialization);
    p << conn.sessionId << Constants::protocol << Rsa::instance().modulus();
    conn.send(p, false);
}

static void handleGameGuardPacket(Connection & conn)
{
    constexpr u32 ignoreGg = 0x0b;
    conn.send(Packet(ServerOpCode::GameGuard) << ignoreGg);
}

static void handleAuthPacket(Connection & conn) try
{
    auto const body = std::span(conn.readBuffer).subspan(sizeof(PacketHeader) + 1);

    // AuthenticationFailed argument meaning
    //  0: ""
    //  1: "System error, please log in again later." (448)
    //  2: "Password does not match this account. Confirm your account information and log in again later." (449+450)
    // ...
    //  4: "Access failed. Please try again later. ." (461+462+463)
    //  5: "Your account information is incorrect.
    //      For more details, please contact our customer service center at http://support.lineage2.com" (453+454)
    //  6: "Access failed. Please try again later. ." (461+462+463)
    //  7: "The account is already in use. Access denied. " (455)
    //  8: "Access failed. Please try again later. ." (461+462+463)
    // ...
    // 12: "Lineage II game services may be used by individuals 15 years of age or older except for PvP servers,
    //      which may only be used by adults 18 years of age and older. (Korea Only)" (456)
    // 13: "Access failed. Please try again later. ." (461+462+463)
    // ...
    // 15: "Due to a large number of users currently accessing our server, your login attempt has failed.
    //      Please wait a little while and attempt to log in again." (1650)
    // 16: "Server under maintenance. Please try again later. " (457)
    // 17: "Please login after changing your temporary password." (396)
    // 18: "Your usage term has expired.  Please visit the official Lineage II website at http://www.lineage2.com to
    //      reactivate your account." (458+459+460)
    // 19: "You have no more time left on your account." (398)
    // 20: "System error." (399)
    // 21: "Access failed. " (461)
    // 22: "This server is reserved for players in Korea.
    //      To use Lineage II game services, please connect to the server in your region. " (621)
    // 23: ""
    // ...
    // 30: "This week’s usage time has finished." (756)
    // 31: "The security card number is invalid." (1243)
    // 32: "Users who have not verified their age cannot log in between 10:00 p.m. and 6:00 a.m." (1242)
    // 33: "This server cannot be accessed by the coupon you are using." (1340)
    // 34: ""
    // 35: "You are using a computer that does not allow you to log in with two accounts at the same time." (1407)

    Rsa::instance().decrypt(body);

    conn.userName = reinterpret_cast<char const *>(body.data() + 0x62);
    conn.password = reinterpret_cast<char const *>(body.data() + 0x70);

    if (conn.password.empty())
        return conn.send(Packet(ServerOpCode::AuthenticationFailed) << 21); // access failed

    std::optional<std::vector<byte>> password;
    L2CPP_F_ASSERT([&] { password = Orm::fetchAccountPassword(conn.userName); },
                   "Failed to fetch password for account '{}", conn.userName);

    if (password)
    {
        std::vector<byte> tmp;
        tmp.append_range(conn.password);
        if (password != tmp)
            return conn.send(Packet(ServerOpCode::AuthenticationFailed) << 2); // password does not match
    }
    else /*if (Config::autoCreateAccountEnabled)*/
    {
        SPDLOG_TRACE("Creating new account '{}'", conn.userName);
        L2CPP_F_ASSERT([&] { Orm::createAccount(conn.userName, conn.password); },
                       "Failed to create new account '{}'", conn.userName);
    }

    std::array<byte, sizeof(u64)> loginKey;
    RAND_bytes(loginKey.data(), static_cast<int>(loginKey.size()));
    conn.send(Packet(ServerOpCode::AuthenticationSuccess) << loginKey);
}
catch (std::exception const & e)
{
    SPDLOG_ERROR("Failed to authenticate user:\n{}", Core::formatExceptionStack(e));
    conn.send(Packet(ServerOpCode::AuthenticationFailed) << 20); // system error
}

static void handleServerListPacket(Connection & conn)
{
    struct ServerInfo
    {
        u8   id             = 1;
        u8   host[4]        = {127, 0, 0, 1};
        u32  port           = 7777;
        u8   ageLimit       = 0;
        bool isPvp          = true;
        u16  curPlayerCount = 0;
        u16  maxPlayerCount = 1'000;
        bool isOnline       = true;
        u32  extra          = 0;
        u8   brackets       = 0;
    } const onlineServer, offlineServer{.id = 2, .isPvp = false, .isOnline = false};

    constexpr u8 serverCount = 2;

    Packet p(ServerOpCode::GameServerList);
    p << serverCount << 0_u8; // ?

    for (auto const srv : {&onlineServer, &offlineServer})
    {
        p
            << srv->id
            << srv->host
            << srv->port
            << srv->ageLimit
            << srv->isPvp
            << srv->curPlayerCount
            << srv->maxPlayerCount
            << srv->isOnline
            << srv->extra
            << srv->brackets
        ;
    }

    conn.send(p);
}

static void handleServerSelectionPacket(Connection & conn)
{
    std::array<byte, sizeof(u64)> gameKey;
    RAND_bytes(gameKey.data(), static_cast<int>(gameKey.size()));
    conn.send(Packet(ServerOpCode::GameServerSelectionSuccess) << gameKey);
}

static void handlePacket(Connection & conn)
{
    auto const payload = std::span(conn.readBuffer).subspan(sizeof(PacketHeader));
    Blowfish::decrypt(payload);
    L2CPP_B_ASSERT(Checksum::verify(payload), "Checksum verification failed: checksums differ");

    auto const opCode = payload[0];
    SPDLOG_INFO("'{}' → 0x{:02x} ({:>3} bytes)", conn.sessionId, opCode, conn.readBuffer.size());

    static std::unordered_map<ClientOpCode, void(*)(Connection &)> const handlers
    {
        { ClientOpCode::Authentication,      &handleAuthPacket            },
        { ClientOpCode::GameServerSelection, &handleServerSelectionPacket },
        { ClientOpCode::GameServerList,      &handleServerListPacket      },
        { ClientOpCode::GameGuard,           &handleGameGuardPacket       },
    };

    if (auto const it = handlers.find(static_cast<ClientOpCode>(opCode)); it != handlers.end()) try
    {
        (*it->second)(conn);
    }
    catch (Core::Exception const & e)
    {
        SPDLOG_ERROR("Something wrong happened during packet handling of connection {}. Closing connection."
                     "\nBacktrace:\n{}", conn.sessionId, Core::formatExceptionStack(e));
        return conn.close();
    }
    else
        SPDLOG_WARN("Received unknown packet type '{}'", opCode);

    conn.asyncReadNextPacket();
}

int main() try
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    spdlog::set_pattern("[%^%R:%S.%e%$] [%L] %v [%s:%#]");
    spdlog::set_level(spdlog::level::trace);
    SPDLOG_INFO("Starting {} version {}", L2CPP_SUBPROJECT_NAME, L2CPP_VERSION);

    SPDLOG_INFO("Initializing database…");
    Database::init({"sql/accounts.sql", "sql/ls_data.sql"});
    SPDLOG_INFO("Database initialization done.");

    std::list<Connection> connections;

    boost::asio::io_context ioContext;
    Network::SocketListener listener(ioContext);

    boost::asio::signal_set signals(ioContext, SIGINT);
    signals.async_wait([&] (auto const &, auto)
    {
        SPDLOG_INFO("Shutdown sequence requested");

        SPDLOG_INFO("Kicking players out");
        for (auto & conn : connections)
        {
            conn.onSocketClosed = nullptr;
            conn.close();
        }

        listener.shutdown();
    });

    auto onSocketAccepted = [&] (boost::asio::ip::tcp::socket && socket)
    {
        SPDLOG_INFO("Client connected!");
        auto & conn = connections.emplace_back(std::move(socket));
        conn.onSocketClosed = [&] { connections.remove(conn);          };
        conn.onPacketRead   = [&] (auto const &) { handlePacket(conn); };

        sendInitPacket(conn);
        conn.asyncReadNextPacket();
    };
    if (!listener.listen(Constants::host, Constants::port, std::move(onSocketAccepted)))
        return EXIT_FAILURE;

    SPDLOG_INFO("Listening for clients on {}:{}", Constants::host, Constants::port);
    ioContext.run();

    SPDLOG_INFO("Goodbye!");
    return EXIT_SUCCESS;
}
catch (Core::Exception const & e)
{
    SPDLOG_CRITICAL("Exception caught:\n{}", Core::formatExceptionStack(e));
    return EXIT_FAILURE;
}
catch (std::exception const & e)
{
    SPDLOG_CRITICAL("Exception caught:\n{}", Core::formatExceptionStack(e));
    return EXIT_FAILURE;
}
catch (...)
{
    SPDLOG_CRITICAL("Unknown exception caught");
    return EXIT_FAILURE;
}
