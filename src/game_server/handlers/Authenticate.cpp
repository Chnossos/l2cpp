/// @author    Chnossos
/// @date      Created on 2026-02-24

// Project includes
#include "../orm/Auth.hpp"
#include "_Common.hpp"

DECLARE_PACKET_HANDLER(CharacterList)

DEFINE_PACKET_HANDLER(Authenticate)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    std::wstring accountName;
    u32 playOk1, playOk2, loginOk1, loginOk2;
    reader >> accountName >> playOk2 >> playOk1 >> loginOk1 >> loginOk2;

    if (auto const id = Orm::fetchAccountId(accountName))
    {
        player.setAccountId(*id);
        player.setAccountName(std::move(accountName));
        player.setPlayOk1(playOk1);

        handleCharacterList(player);
    }
    else
        player.connection().send(Packet(0x14, "AuthenticateFail") << 1);
}
