/// @author    Chnossos
/// @date      Created on 2026-03-31

#include "Position.hpp"

// Projet includes
#include <l2cpp/network/Packet.hpp>
#include <l2cpp/network/PacketReader.hpp>

DEFINE_PACKET_SERIALIZATION_OPERATOR(Position, pos)
{
    return p << pos.x << pos.y << pos.z;
}

DEFINE_PACKET_DESERIALIZATION_OPERATOR(Position, pos)
{
    return r >> pos.x >> pos.y >> pos.z;
}
