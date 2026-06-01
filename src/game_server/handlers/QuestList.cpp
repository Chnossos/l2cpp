/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(QuestList)
{
    constexpr u16 questAmount = 0, questItems = 0;
    player.connection().send(Packet(0x80, "QuestList") << questAmount << questItems);
}
