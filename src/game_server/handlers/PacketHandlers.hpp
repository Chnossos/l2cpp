/// @author    Chnossos
/// @date      Created on 2026-02-24

#pragma once

// Project includes
#include "../Typedefs.hpp"

// C++ includes
#include <string_view>

class Player;

using PacketHandler = void(*)(Player &);

extern std::unordered_map<PacketOpCode, std::pair<PacketHandler, std::string_view>> gPacketHandlers;
