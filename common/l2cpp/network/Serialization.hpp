/// @author    Chnossos
/// @date      Created on 2026-03-03

#pragma once

// Forward-declare for the macros to work with no additional includes
namespace Network { class Packet; class PacketReader; }

#define DEFINE_CUSTOM_PACKET_SERIALIZATION_OPERATOR(P, T, t) \
    Network::Packet & operator<<(P & p, T const & t)

#define DEFINE_PACKET_SERIALIZATION_OPERATOR(T, t) \
    DEFINE_CUSTOM_PACKET_SERIALIZATION_OPERATOR(Network::Packet, T, t)

#define DEFINE_PACKET_DESERIALIZATION_OPERATOR(T, t) \
    Network::PacketReader & operator>>(Network::PacketReader & r, T & t)

#define DECLARE_CUSTOM_PACKET_SERIALIZATION_OPERATOR(P, T) \
    friend DEFINE_CUSTOM_PACKET_SERIALIZATION_OPERATOR(P, T,)

#define DECLARE_PACKET_SERIALIZATION_OPERATOR(T) \
    friend DEFINE_PACKET_SERIALIZATION_OPERATOR(T,)

#define DECLARE_PACKET_DESERIALIZATION_OPERATOR(T) \
    friend DEFINE_PACKET_DESERIALIZATION_OPERATOR(T,)

#define DECLARE_PACKET_SERIALIZATION_OPERATORS(T) \
    DECLARE_PACKET_SERIALIZATION_OPERATOR(T);     \
    DECLARE_PACKET_DESERIALIZATION_OPERATOR(T)
