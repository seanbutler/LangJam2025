#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "vm.h"

namespace vm32 {

// Simple binary format:
//   magic[8]   = "LJBC\r\n\x1A\n"
//   version    = u32 (currently 1)
//   cellCount  = u32
//   cells      = cellCount * u32 (little-endian)
//
// Cells are the VM's instruction/data cells (u32) exactly as used by VM::load().

bool saveBytecodeToFile(const std::vector<u32>& cells, const std::string& path, std::string* outError = nullptr);

bool loadBytecodeFromFile(const std::string& path, std::vector<u32>& outCells, std::string* outError = nullptr);

} // namespace vm32
