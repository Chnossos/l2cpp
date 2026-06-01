/// @author    Chnossos
/// @date      Created on 2026-04-09

#include "UiGaugePacket.hpp"

using Network::Packets::Server::UiGaugePacket;

UiGaugePacket::UiGaugePacket(GaugeColor const color, ClockDuration const duration)
    : Packet(0x6d, "UiGauge")
{
    auto const d = std::chrono::floor<std::chrono::milliseconds>(duration);

    *this
        << color
        << d // cur value
        << d // max value
    ;
}
