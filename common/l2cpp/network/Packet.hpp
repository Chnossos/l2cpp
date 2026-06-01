/// @author    Chnossos
/// @date      Created on 2026-02-17

#pragma once

// Project includes
#include "../Pimpl.hpp"
#include "../Typedefs.hpp"
#include "../utils/Traits.hpp"

// C++ includes
#include <chrono>
#include <span>
#include <string_view>

namespace l2cpp::Network { class Packet; template<u16> struct HeaderOnlyPacket; }

class l2cpp::Network::Packet
{
public:
    explicit Packet(PacketOpCode opCode, std::string_view name = {});

    /// Enables shallow copy.
    Packet(Packet const & other);

    /// Permits conversion from any Packet enumerations that has the correct underlying type size
    template<typename E>
    requires std::is_enum_v<E> && Utils::Traits::isAnyOf<std::underlying_type_t<E>, byte, PacketOpCode>
    explicit Packet(E e): Packet(std::to_underlying(e)) {}

    Packet(Packet &&) noexcept = default;
    Packet & operator=(Packet &&) noexcept = default;
    virtual ~Packet();

public:
    /// @returns A read-only span of the whole buffer.
    auto buffer() const -> std::span<byte const>;

    /// @returns Size of the whole buffer.
    auto size() const -> size_t;

    /// @returns Opcode of the packet, if available.
    auto opCode() const -> PacketOpCode;

    /// @returns A span of the buffer minus the initial size (thus including the opCode).
    auto body() -> std::span<byte>;

    /// @returns A const view of the buffer minus the initial size (thus including the opCode).
    auto body() const -> std::span<byte const>;

    /// @returns Size of the body (buffer size minus the initial size).
    auto bodySize() const -> size_t;

    auto name() const -> std::string_view;

public:
    /// Appends a span of bytes to the packet.
    /// @warning Appending to a finalized packet won't work!
    Packet & operator<<(std::span<byte const>);

    /// Allows to append any "basic" type as bytes to the packet.
    template<typename T> requires std::integral<T> || std::floating_point<T> || std::is_enum_v<T>
    Packet & operator<<(T t)
    {
        if constexpr (std::is_enum_v<T>)
            return append(&t, sizeof(std::underlying_type_t<T>));
        else
            return append(&t, sizeof(T));
    }

    /// Appends a contiguous array of integrals to the packet.
    template<typename T, size_t N> requires (!std::is_same_v<T, byte> && std::integral<T> || std::floating_point<T>)
    Packet & operator<<(std::array<T, N> const & a)  {return append(a.data(),   a.size()   * sizeof(T));               }
    Packet & operator<<(std::string_view const str)  {return append(str.data(), str.size() * sizeof(char))    <<  '\0';}
    Packet & operator<<(std::wstring_view const str) {return append(str.data(), str.size() * sizeof(wchar_t)) << L'\0';}

    template<typename Rep, typename Period>
    Packet & operator<<(std::chrono::duration<Rep, Period> const & d) { return *this << static_cast<u32>(d.count()); }

protected:
    template<typename T> requires std::integral<T>
    void appendCounterAndStoreOffset(T & t, T initialSize = T{})
    {
        t = static_cast<T>(size());
        *this << initialSize;
    }

    template<typename T> requires Utils::Traits::isAnyOf<T, u16, u32>
    auto counterAtOffset(T const offset) -> T & {
        return *static_cast<T *>(counterAtOffset(static_cast<size_t>(offset)));
    }

    template<typename T> requires Utils::Traits::isAnyOf<T, u16, u32>
    auto counterAtOffset(T const offset) const -> T {
        return *static_cast<T const *>(counterAtOffset(static_cast<size_t>(offset)));
    }

    void erase(size_t size);

private:
    auto append(void const * ptr, size_t const sz) -> Packet & {
        return operator<<({static_cast<byte const *>(ptr), sz});
    }

    auto counterAtOffset(size_t offset) -> void *;
    auto counterAtOffset(size_t offset) const -> void const *;

private:
    struct Impl;
    Pimpl<Impl> _impl;
};

template<typename T> requires std::is_class_v<T>
l2cpp::Network::Packet & operator<<(l2cpp::Network::Packet && p, T const & t)
{
    return p << t;
}
