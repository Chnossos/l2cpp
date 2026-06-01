/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include <gs/handlers/_Common.hpp>

DEFINE_PACKET_HANDLER(ManorList)
{
    static std::array const manors
    {
        L"gludio"sv,
        L"dion"sv,
        L"giran"sv,
        L"oren"sv,
        L"aden"sv,
        L"innadril"sv,
        L"goddard"sv,
    };

    Packet p(0x1bfe, "ManorList");
    p
        << 0_u8 // ?
        << static_cast<u32>(manors.size())
    ;

    for (u32 i = 0; i < manors.size(); ++i)
        p << i + 1 << manors[i];

    player.connection().send(p);
}
