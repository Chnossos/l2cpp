/// @author    Chnossos
/// @date      Created on 2026-05-07

#pragma once

// C++ includes
#include <filesystem>

namespace Utils
{
    auto readWholeTextFile(std::filesystem::path const &) -> std::string;
    auto readWhileBinaryFile(std::filesystem::path const &) -> std::vector<unsigned char>;
}
