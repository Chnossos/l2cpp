/// @author    Chnossos
/// @date      Created on 2026-02-18

#pragma once

// Project includes
#include <common/Typedefs.hpp>

// C++ includes
#include <span>

namespace Blowfish
{
    void encrypt(std::span<byte> data);
    void decrypt(std::span<byte> data);
};
