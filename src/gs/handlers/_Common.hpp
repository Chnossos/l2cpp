/// @author    Chnossos
/// @date      Created on 2026-02-24

#pragma once

// Project includes
#include "../Player.hpp"
#include "../network/Connection.hpp"
#include <common/core/Exception.hpp>
#include <common/network/Packet.hpp>
#include <common/network/PacketReader.hpp>

// Third-party includes
#include <spdlog/spdlog.h>

using namespace Network;

// Forward-declare
namespace Network::Packets::Server {}
using namespace Network::Packets::Server;

#define DECLARE_PACKET_HANDLER(name) namespace Handler { void handle ## name(Player & player); }

#define DEFINE_PACKET_HANDLER(name) \
    DECLARE_PACKET_HANDLER(name)    \
    void Handler::handle ## name(Player & player)
