/// @author    Chnossos
/// @date      Created on 2026-02-16

#include "Misc.hpp"

// C++ includes
#include <format>

std::string Utils::hexdump(void const * const ptr, size_t const size)
{
    // line format: "012345: 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF 0123456789ABCDEF"
    constexpr size_t maxLineWidth = 6 + 2 + 16 * 3 + 16;

    std::string result;
    if (ptr && size)
    {
        result.reserve(size / 16 * maxLineWidth + (size % 16 ? maxLineWidth : 0));

        auto const buf = static_cast<unsigned char const * const>(ptr);
        for (size_t i = 0; i < size; i += 16)
        {
            result += std::format("{:06X}: ", i);

            for (size_t j = 0; j < 16; ++j)
                result += i + j < size ? std::format("{:02X} ", buf[i + j]) : "__ ";

            for (size_t j = 0; j < 16; ++j)
            {
                if (i + j < size)
                    result += std::isprint(buf[i + j]) ? buf[i + j] : '.';
                else
                    result += ' ';
            }

            result += '\n';
        }

        result.pop_back(); // remove trailing newline
    }
    return result;
}
