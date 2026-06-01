/// @author    Chnossos
/// @date      Created on 2026-02-18

#include "Blowfish.hpp"

// Project includes
#include "../Constants.hpp"

// Third-party includes
#include <openssl/blowfish.h>

// C++ includes
#include <algorithm>

namespace
{
    struct BlowfishKey
    {
        BF_KEY bfKey;

        explicit BlowfishKey(std::span<byte const> const key) {
            BF_set_key(&bfKey, static_cast<int>(key.size()), key.data());
        }

        // ReSharper disable once CppNonExplicitConversionOperator
        operator BF_KEY const *() const { return &bfKey; }
    };

    void apply(std::span<byte> const s, bool const mode)
    {
        static BlowfishKey const key(Constants::blowfishToken);

        constexpr size_t chunkSize = sizeof(u64);

        for (size_t i = 0; i + chunkSize <= s.size(); i += chunkSize)
        {
            std::ranges::reverse(s.data() + i + 0, s.data() + i + 4);
            std::ranges::reverse(s.data() + i + 4, s.data() + i + 8);

            BF_ecb_encrypt(s.data() + i, s.data() + i, key, mode);

            std::ranges::reverse(s.data() + i + 0, s.data() + i + 4);
            std::ranges::reverse(s.data() + i + 4, s.data() + i + 8);
        }
    }
}

void Blowfish::encrypt(std::span<byte> data)
{
    ::apply(std::move(data), BF_ENCRYPT);
}

void Blowfish::decrypt(std::span<byte> data)
{
    ::apply(std::move(data), BF_DECRYPT);
}
