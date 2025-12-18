#include "bytecode_io.h"

#include <array>
#include <fstream>
#include <limits>

namespace vm32 {

namespace {

constexpr std::array<std::uint8_t, 8> kMagic = { 'L','J','B','C','\r','\n',0x1A,'\n' };
constexpr u32 kVersion = 1;

void setError(std::string* outError, const std::string& msg) {
    if (outError) *outError = msg;
}

bool writeU32LE(std::ostream& os, u32 v) {
    const std::uint8_t b0 = static_cast<std::uint8_t>(v & 0xFF);
    const std::uint8_t b1 = static_cast<std::uint8_t>((v >> 8) & 0xFF);
    const std::uint8_t b2 = static_cast<std::uint8_t>((v >> 16) & 0xFF);
    const std::uint8_t b3 = static_cast<std::uint8_t>((v >> 24) & 0xFF);
    os.put(static_cast<char>(b0));
    os.put(static_cast<char>(b1));
    os.put(static_cast<char>(b2));
    os.put(static_cast<char>(b3));
    return static_cast<bool>(os);
}

bool readU32LE(std::istream& is, u32& out) {
    std::uint8_t b[4];
    if (!is.read(reinterpret_cast<char*>(b), 4)) return false;
    out = static_cast<u32>(b[0]) |
          (static_cast<u32>(b[1]) << 8) |
          (static_cast<u32>(b[2]) << 16) |
          (static_cast<u32>(b[3]) << 24);
    return true;
}

} // namespace

bool saveBytecodeToFile(const std::vector<u32>& cells, const std::string& path, std::string* outError) {
    std::ofstream f(path, std::ios::binary);
    if (!f) {
        setError(outError, "Failed to open for write: " + path);
        return false;
    }

    f.write(reinterpret_cast<const char*>(kMagic.data()), static_cast<std::streamsize>(kMagic.size()));
    if (!f) {
        setError(outError, "Failed to write header magic");
        return false;
    }

    if (!writeU32LE(f, kVersion)) {
        setError(outError, "Failed to write version");
        return false;
    }

    if (cells.size() > static_cast<std::size_t>(std::numeric_limits<u32>::max())) {
        setError(outError, "Too many cells to serialize");
        return false;
    }

    if (!writeU32LE(f, static_cast<u32>(cells.size()))) {
        setError(outError, "Failed to write cell count");
        return false;
    }

    for (u32 v : cells) {
        if (!writeU32LE(f, v)) {
            setError(outError, "Failed to write cell data");
            return false;
        }
    }

    return true;
}

bool loadBytecodeFromFile(const std::string& path, std::vector<u32>& outCells, std::string* outError) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        setError(outError, "Failed to open for read: " + path);
        return false;
    }

    std::array<std::uint8_t, 8> magic{};
    if (!f.read(reinterpret_cast<char*>(magic.data()), static_cast<std::streamsize>(magic.size()))) {
        setError(outError, "Failed to read header magic");
        return false;
    }
    if (magic != kMagic) {
        setError(outError, "Invalid bytecode magic");
        return false;
    }

    u32 version = 0;
    if (!readU32LE(f, version)) {
        setError(outError, "Failed to read version");
        return false;
    }
    if (version != kVersion) {
        setError(outError, "Unsupported bytecode version: " + std::to_string(version));
        return false;
    }

    u32 count = 0;
    if (!readU32LE(f, count)) {
        setError(outError, "Failed to read cell count");
        return false;
    }

    outCells.clear();
    outCells.reserve(static_cast<std::size_t>(count));

    for (u32 i = 0; i < count; ++i) {
        u32 v = 0;
        if (!readU32LE(f, v)) {
            setError(outError, "Unexpected EOF while reading cells");
            return false;
        }
        outCells.push_back(v);
    }

    return true;
}

} // namespace vm32
