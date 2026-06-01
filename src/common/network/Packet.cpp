/// @author    Chnossos
/// @date      Created on 2026-02-17

#include "Packet.hpp"

// Project includes
#include <common/details/Pimpl.hpp>

// C++ includes
#include <vector>

using Network::Packet;

struct Packet::Impl
{
    std::vector<byte> buffer;
    PacketOpCode      opCode;
    std::string_view  name;

    Impl(PacketOpCode opCode_, std::string_view name_);
    Impl(Impl const & other);

    void append(std::span<byte const> data);
    void erase(size_t size);

private:
    void writeSize();
};

template class Pimpl<Packet::Impl>;

Packet::Impl::Impl(PacketOpCode const opCode_, std::string_view const name_)
    : buffer(sizeof(PacketHeader))
    , opCode(opCode_)
    , name(name_)
{}

Packet::Impl::Impl(Impl const & other)
    : buffer(other.buffer)
    , opCode(other.opCode)
    , name(other.name)
{}

void Packet::Impl::append(std::span<byte const> const data)
{
    buffer.append_range(data);
    writeSize();
}

void Packet::Impl::erase(size_t const size)
{
    buffer.erase(buffer.cend() - size);
    writeSize();
}

void Packet::Impl::writeSize()
{
    // Write total size on the first two bytes of the buffer
    *reinterpret_cast<PacketHeader *>(buffer.data()) = static_cast<PacketHeader>(buffer.size());
}

// ---------------------------------------------------------------------------------------------------------------------

Packet::Packet(PacketOpCode const opCode, std::string_view name)
    : _impl(opCode, name)
{
    if (opCode > 0xff)
        *this << opCode;
    else
        *this << static_cast<byte>(opCode);
}

Packet::Packet(Packet const & other)
    : _impl(*other._impl)
{}

Packet::~Packet() = default;

auto Packet::buffer()      const -> std::span<byte const> { return _impl->buffer;        }
auto Packet::size()        const -> size_t                { return _impl->buffer.size(); }
auto Packet::opCode()      const -> PacketOpCode          { return _impl->opCode;        }

auto Packet::body()       -> std::span<byte>       { return {_impl->buffer.data() + sizeof(PacketHeader), bodySize()}; }
auto Packet::body() const -> std::span<byte const> { return {_impl->buffer.data() + sizeof(PacketHeader), bodySize()}; }
auto Packet::bodySize() const -> size_t            { return  _impl->buffer.size() - sizeof(PacketHeader);              }

auto Packet::name() const -> std::string_view { return _impl->name; }

Packet & Packet::operator<<(std::span<byte const> const data)
{
    _impl->append(data);
    return *this;
}

void Packet::erase(size_t const size)
{
    _impl->erase(std::max(size, this->size() - sizeof(PacketHeader) - (_impl->opCode > 0xff ? 2 : 1)));
}

auto Packet::counterAtOffset(size_t const offset)       -> void       * { return _impl->buffer.data() + offset; }
auto Packet::counterAtOffset(size_t const offset) const -> void const * { return _impl->buffer.data() + offset; }
