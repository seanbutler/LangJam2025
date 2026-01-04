#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace vm32 {

using u8  = std::uint8_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;

enum class Op : u8 {
    HALT = 0x00,
    PUSHI = 0x01,     // push immediate i32 (4 bytes)
    POP   = 0x02,

    ADD = 0x10,
    SUB = 0x11,
    MUL = 0x12,
    DIV = 0x13,       // signed int division
    MOD = 0x14,       // signed int modulo
    NEG = 0x15,
    DUP = 0x16,
    SWAP= 0x17,
    OVER= 0x18,     // duplicate second stack item

    PRINT = 0x20,     // print top as i32

    JMP = 0x30,       // absolute byte offset (u32)
    JZ  = 0x31,       // pop cond; if zero -> jump abs(u32)
    JNZ = 0x32,       // pop cond; if non-zero -> jump abs(u32)

    CMP_EQ = 0x40,    // push 1 if a==b else 0
    CMP_LT = 0x41,    // push 1 if a<b else 0
    CMP_GT = 0x42,    // push 1 if a>b else 0

    LOAD  = 0x50,    // load from mem[u32 addr]
    STORE = 0x51,    // store to mem[u32 addr] (pop value)
    STORE_IND = 0x52 // store to mem[addr] where addr is popped from stack (value below it)
};

} // namespace vm32
