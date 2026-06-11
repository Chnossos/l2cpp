/// @author    Chnossos
/// @date      Created on 2026-03-10

#pragma once

// Project includes
#include <gs/game/World.hpp>
#include <gs/game/actions/SocialAction.hpp>
#include <gs/game/actions/TeleportAction.hpp>
#include <gs/game/actor/Character.hpp>
#include <gs/game/actor/Monster.hpp>
#include <gs/game/components/CharacterStatus.hpp>
#include <gs/game/components/KnownActors.hpp>
#include <gs/game/components/PlayerAppearance.hpp>
#include <gs/game/components/SkillDirectory.hpp>
#include <gs/game/directories/NpcDirectory.hpp>
#include <gs/game/lobby/CharacterCreationParameters.hpp>
#include <gs/handlers/_Common.hpp>
#include <gs/network/packets/server/chat/ChatSystemSayPacket.hpp>
#include <gs/network/packets/server/movement/ObjectTeleportPacket.hpp>
#include <gs/network/packets/server/skill/SkillListPacket.hpp>
#include <gs/network/packets/server/status/CharacterStatusUpdateBroadcastPacket.hpp>
#include <gs/network/packets/server/status/NpcStatusUpdatePacket.hpp>

// Third-party includes
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

// C++ includes
#include <format>
#include <fstream>
#include <string_view>

template<typename CharT> requires Utils::Traits::isAnyOf<CharT, char, wchar_t>
auto toInt(std::basic_string_view<CharT> const & view)
{
    if constexpr (std::is_same_v<CharT, char>)
        return std::stoi(std::string(view));
    else
        return std::stoi(std::wstring(view));
}

static std::wstring readWholeFile(std::string_view path)
{
    std::wstring content;
    std::string  asciiContent;

    if (std::ifstream file(path.data()); file)
    {
        asciiContent.resize(file.seekg(0, std::ios::end).tellg());
        file.seekg(0);
        file.read(asciiContent.data(), asciiContent.size());
    }
    else
    {
        constexpr auto fmt = R"(
<html>
  <body>
    <font color="BF0000">Failed to open "</font><font color="LEVEL">{}</font><font color="BF0000">".</font>
  </body>
</html>
)";
        asciiContent = std::format(fmt, path);
    }

    content.resize(asciiContent.size());
    for (size_t i = 0; auto const c : asciiContent)
        *(reinterpret_cast<byte *>(content.data()) + i++ * 2) = c;

    return content;
}

DEFINE_PACKET_HANDLER(ChatAdminCommand) try
{
    PacketReader reader(player.connection().readBuffer().subspan(3));

    std::wstring text;
    reader >> text;

    auto & c = *player.currentCharacter();

    boost::algorithm::trim_right(text);
    std::vector<std::wstring_view> args;
    boost::algorithm::split(args, text, boost::algorithm::is_any_of(L" "), boost::token_compress_on);

    if (args[0] == L"/")
    {
        Packet p(0x0f);
        p
            << 0 // NPC id
            << std::vformat(readWholeFile("data/test.html"), std::make_wformat_args(text))
        ;
        player.connection().send(p);
    }
    else if (args[0] == L"spawn")
    {
        OptRef<Npc> npc;

        if (args.size() == 1)
            npc = World::addNpc(1);
        else if (std::isdigit(args[1][0]))
            npc = World::addNpc(toInt(args[1]));
        else if (auto const infos = NpcDirectory::find(args[1]); !infos.empty())
            npc = World::addNpc(infos[0].get().id);

        if (npc)
        {
            npc->setPosition(c.position());
            World::broadcastAround(npc, NpcStatusUpdatePacket(npc));
        }
        else
        {
            ChatSystemSayPacket p{SystemMessageId::_1_2};
            if (args.size() > 1)
                p << std::format(L"Failed to spawn npc/monster \"{}\":", args[1]);
            else
                p << std::format("Failed to spawn npc/monster:");

            p << "not found.";
            player.connection().send(std::move(p));
        }
    }
    else if (args[0] == L"learn")
    {
        std::wstring cmd;
        SkillId id;
        SkillLevel level = 1;

        std::wistringstream iss(text);
        iss >> cmd >> id >> level;

        auto & skills = c.skills();
        skills.learn(id, level);
        player.connection().send(SkillListPacket(skills));
    }
    else if (args[0] == L"char")
    {
        auto & d = World::addCharacter({
            .name               = L"dummy",
            .sex                = Sex::Female,
            .startingProfession = Profession::ElvenMystic,
            .position           = c.position(),
        });
        World::broadcastAround(d, CharacterStatusUpdateBroadcastPacket(d));
    }
    else if (args[0] == L"xp" || args[0] == L"sp")
    {
        std::wistringstream iss(text);
        std::wstring cmd;
        u32 nbr = 0;
        iss >> cmd >> nbr;
        if (cmd == L"xp")
            c.status().setXp(nbr);
        else
            c.status().setSp(nbr);
    }
    else if (args[0] == L"social")
    {
        std::wistringstream iss(text);
        std::wstring cmd;
        u32 actionId;
        iss >> cmd >> actionId;
        c.doNext<SocialAction>(static_cast<SocialActionId>(actionId));
    }
    else if (args[0] == L"goto")
    {
        std::optional<Position> pos;
        TeleportationStyle      style{};
        ChatSystemSayPacket     msg{SystemMessageId::_1_2};

        if (args.size() < 2)
        {
            msg << "Usages:" << R"(
goto object_id
goto pos_x pos_y pos_z [style])";
        }
        if (args.size() >= 2 && std::isdigit(args[1][0]))
        {
            auto const id = toInt(args[1]);
            if (auto const actor = World::actor(id))
            {
                auto const orientation = c.position().orientation;
                pos.emplace(actor->position());
                pos->orientation = orientation; // keep current orientation
                style = static_cast<TeleportationStyle>(args.size() == 3 ? args[2][0] - L'0' : 0);
            }
            else
                msg << "Failed:" << std::format("could not find actor from id '{}'", id);
        }
        else if (args.size() < 3 || args.size() > 4)
            msg << "Usage:" << "goto pos_x pos_y pos_z [style]";
        else
        {
            pos.emplace(toInt(args[1]), toInt(args[2]), toInt(args[3]), c.position().orientation);
            style = static_cast<TeleportationStyle>(args.size() == 4 ? args[3][0] - L'0' : 0);
        }

        if (msg.argumentCount() > 0)
            player.connection().send(msg);

        if (pos)
        {
            c.cancelAction(); // Prevent the teleport from being queued
            c.doNext<TeleportAction>(*pos, style, true);
        }
    }
}
catch (Core::Exception const & e)
{
    SPDLOG_ERROR("Admin command failed:\n{}", Core::formatExceptionStack(e));

    ChatSystemSayPacket msg{SystemMessageId::_1_2};
    msg << "Failed:" << e.what();
    player.connection().send(msg);
}
