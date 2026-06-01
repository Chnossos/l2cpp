/// @author    Chnossos
/// @date      Created on 2026-03-03

// Project includes
#include "../game/actor/Character.hpp"
#include "../game/ui/ShortcutBar.hpp"
#include "_Common.hpp"

DEFINE_PACKET_HANDLER(UiShortcutUnset)
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    u32 index;
    reader >> index;
    player.currentCharacter()->shortcutBar().unset(index);
}
