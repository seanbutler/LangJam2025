#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "../bytecode/opcodes.h"

namespace vm32 {

struct Result {
    bool ok{true};
    std::string error;
    std::size_t steps{0};
};

class VM {
public:
    explicit VM(std::size_t stackCapacity = 1024);

    void load(const std::vector<u32>& codeCells);
    void reset();

    Result run(std::size_t maxSteps = 1'000'000);
    Result step();

    u32 ip() const { return m_ip; }
    u32 sp() const { return m_sp; }
    std::size_t memSize() const { return m_mem.size(); }
    i32 memAt(u32 addr) const { return m_mem.at(addr); }

    // Host-side I/O helpers (for SDL / embedding)
    void setKeyboardState(u32 mask);
    u32 keyboardState() const;

    // Public memory layout constants (cell addresses)
    static constexpr u32 MEM_SIZE  = 0x10000;        // cells
    static constexpr u32 CODE_BASE = 0x00000;       // base of code region
    static constexpr u32 DATA_BASE = 0x00800;       // base of data region
    static constexpr u32 STACK_BASE = 0x01000;      // base of stack in memory
    static constexpr u32 STACK_LIMIT = MEM_SIZE;   // end of stack region (exclusive)

    // Memory-mapped framebuffer (32-bit ARGB8888 per cell)
    static constexpr u32 FB_WIDTH  = 256;
    static constexpr u32 FB_HEIGHT = 192;
    static constexpr u32 FB_SIZE   = FB_WIDTH * FB_HEIGHT; // cells
    static constexpr u32 FB_BASE   = MEM_SIZE - FB_SIZE;   // base cell address of framebuffer

    // Memory-mapped input registers (read by VM bytecode via LOAD)
    // Layout: a small I/O page immediately below the framebuffer.
    static constexpr u32 IO_SIZE = 16;                // cells
    static constexpr u32 IO_BASE = FB_BASE - IO_SIZE; // base cell address of I/O page

    // Keyboard/gamepad-style bitmask register
    static constexpr u32 KB_BASE       = IO_BASE;
    static constexpr u32 KB_STATE_ADDR = KB_BASE + 0; // u32 bitmask of currently-held buttons

    // Bit assignments for KB_STATE_ADDR
    static constexpr u32 KB_UP     = 1u << 0;
    static constexpr u32 KB_DOWN   = 1u << 1;
    static constexpr u32 KB_LEFT   = 1u << 2;
    static constexpr u32 KB_RIGHT  = 1u << 3;
    static constexpr u32 KB_A      = 1u << 4;
    static constexpr u32 KB_B      = 1u << 5;
    static constexpr u32 KB_X      = 1u << 6;
    static constexpr u32 KB_Y      = 1u << 7;
    static constexpr u32 KB_L      = 1u << 8;
    static constexpr u32 KB_R      = 1u << 9;
    static constexpr u32 KB_SELECT = 1u << 10;
    static constexpr u32 KB_START  = 1u << 11;

private:
    bool fetchCell(u32& out);
    bool push(i32 v);
    bool pop(i32& out);
    bool peek2(i32& a, i32& b); // a=top, b=second from top

    std::vector<i32> m_mem; // unified memory (cells of i32)
    u32 m_ip{0};
    u32 m_sp{0}; // stack pointer: index of next free slot (top is sp-1)
    std::size_t m_stackCap{0}; // logical stack capacity (for compatibility)
};

} // namespace vm32
