/// @author    Chnossos
/// @date      Created on 2026-04-26

#pragma once

// Project includes
#include "../../../../game/skill/SkillUid.hpp"

#include <l2cpp/network/Packet.hpp>

struct SystemMessageArgument
{
    SystemMessageArgument() noexcept = default;
    SystemMessageArgument(SystemMessageArgument const &) noexcept = default;
    SystemMessageArgument & operator=(SystemMessageArgument const &) noexcept = default;
    SystemMessageArgument(SystemMessageArgument &&) noexcept = default;
    SystemMessageArgument & operator=(SystemMessageArgument &&) noexcept = default;
    virtual ~SystemMessageArgument() noexcept = default;

    virtual void serialize(Network::Packet &) const = 0;
};

namespace details
{
    enum class SystemMessageArgumentType
    {
        Text      = 0,
        Number    = 1,
        NpcName   = 2,
        ItemName  = 3,
        SkillName = 4,
    };

    template<SystemMessageArgumentType t, typename Value>
    struct SystemMessageArgumentImpl final : public SystemMessageArgument
    {
        SystemMessageArgumentType const type{t};
        Value value;

        explicit SystemMessageArgumentImpl(Value const v)
            : value(v)
        {}

        void serialize(Network::Packet & p) const override { p << type << value; }
    };
}

#define DECLARE_SYS_MSG_ARG_TYPE(Type, ValueType)                                                             \
    template struct details::SystemMessageArgumentImpl<details::SystemMessageArgumentType::Type, ValueType>;  \
    namespace SysMsgArg {                                                                                     \
        using Type = details::SystemMessageArgumentImpl<details::SystemMessageArgumentType::Type, ValueType>; \
    }

DECLARE_SYS_MSG_ARG_TYPE(Text,      std::wstring_view);
DECLARE_SYS_MSG_ARG_TYPE(Number,    u32);
DECLARE_SYS_MSG_ARG_TYPE(NpcName,   u32);
DECLARE_SYS_MSG_ARG_TYPE(ItemName,  u32);
DECLARE_SYS_MSG_ARG_TYPE(SkillName, SkillUid);

#undef DECLARE_SYS_MSG_ARG_TYPE
