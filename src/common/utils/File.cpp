/// @author    Chnossos
/// @date      Created on 2026-05-07

#include "File.hpp"

// Project includes
#include <common/core/Exception.hpp>

// C++ includes
#include <cstring>
#include <fstream>

namespace
{
    template<class Buffer>
    auto readWholeFile(std::filesystem::path const & path, std::ios_base::openmode const mode) -> Buffer
    {
        std::ifstream file(path, mode);
        L2CPP_B_ASSERT(file, "Failed to open file '{}': {}", path.string(), std::strerror(errno));

        file.ignore(std::numeric_limits<std::streamsize>::max());
        Buffer buffer(static_cast<size_t>(file.gcount()), 0);
        file.clear(); // Since ignore will have set eof.
        file.seekg(0, file.beg);
        file.read(reinterpret_cast<char *>(buffer.data()), buffer.size());
        return buffer;
    }
}

auto Utils::readWholeTextFile(std::filesystem::path const & path) -> std::string
{
    return readWholeFile<std::string>(path, std::ios_base::in);
}

auto Utils::readWhileBinaryFile(std::filesystem::path const & path) -> std::vector<unsigned char>
{
    return readWholeFile<std::vector<unsigned char>>(path, std::ios_base::in | std::ios_base::binary);
}
