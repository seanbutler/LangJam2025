#include "vm.h"
#include <cstdio>

namespace vm32 {

VM::VM(std::size_t stackCapacity) : m_stackCap(stackCapacity) {
    m_mem.assign(MEM_SIZE, 0);
    m_sp = STACK_BASE;
    m_ip = CODE_BASE;
}

void VM::load(const std::vector<u32>& codeCells) {
    reset();
    for (u32 i = 0; i < codeCells.size() && (CODE_BASE + i) < MEM_SIZE; ++i) {
        m_mem[CODE_BASE + i] = static_cast<i32>(codeCells[i]);
    }
}

void VM::reset() {
    std::fill(m_mem.begin(), m_mem.end(), 0);
    m_sp = STACK_BASE;
    m_ip = CODE_BASE;
}

void VM::setKeyboardState(u32 mask) {
    if (KB_STATE_ADDR < m_mem.size()) {
        m_mem[KB_STATE_ADDR] = static_cast<i32>(mask);
    }
}

u32 VM::keyboardState() const {
    if (KB_STATE_ADDR < m_mem.size()) {
        return static_cast<u32>(m_mem[KB_STATE_ADDR]);
    }
    return 0;
}

bool VM::fetchCell(u32& out) {
    if (m_ip >= MEM_SIZE) return false;
    out = static_cast<u32>(m_mem[m_ip++]);
    return true;
}

bool VM::push(i32 v) {
    u32 capEnd = STACK_BASE;
    if (m_stackCap > 0) {
        const u32 cap = static_cast<u32>(m_stackCap);
        capEnd = (cap > (STACK_LIMIT - STACK_BASE)) ? STACK_LIMIT : (STACK_BASE + cap);
    } else {
        capEnd = STACK_LIMIT;
    }
    if (m_sp >= capEnd) return false;
    m_mem[m_sp++] = v;
    return true;
}

bool VM::pop(i32& out) {
    if (m_sp <= STACK_BASE) return false;
    out = m_mem[--m_sp];
    return true;
}

bool VM::peek2(i32& a, i32& b) {
    if (m_sp - STACK_BASE < 2) return false;
    a = m_mem[m_sp - 1];
    b = m_mem[m_sp - 2];
    return true;
}

Result VM::run(std::size_t maxSteps) {
    Result r{};
    for (std::size_t i = 0; i < maxSteps; ++i) {
        r = step();
        if (!r.ok) return r;
        if (r.steps == 0) { // HALT
            return r;
        }
    }
    r.ok = false;
    r.error = "Exceeded maxSteps";
    return r;
}

Result VM::step() {
    Result r{};
    u32 opCell;
    if (!fetchCell(opCell)) { r.ok = false; r.error = "IP out of range"; return r; }
    Op op = static_cast<Op>(opCell);
    r.steps = 1;

    i32 a, b;

    switch (op) {
        case Op::HALT:
            r.steps = 0;
            return r;
        case Op::PUSHI: {
            u32 imm;
            if (!fetchCell(imm)) { r.ok = false; r.error = "Truncated PUSHI"; return r; }
            if (!push(static_cast<i32>(imm))) { r.ok = false; r.error = "Stack overflow"; return r; }
            return r;
        }
        case Op::POP:
            if (!pop(a)) { r.ok = false; r.error = "Stack underflow (POP)"; }
            return r;
        case Op::DUP:
            if (!pop(a)) { r.ok = false; r.error = "Stack underflow (DUP)"; return r; }
            if (!push(a) || !push(a)) { r.ok = false; r.error = "Stack overflow (DUP)"; }
            return r;
        case Op::SWAP:
            if (m_sp - STACK_BASE < 2) { r.ok = false; r.error = "Stack underflow (SWAP)"; return r; }
            std::swap(m_mem[m_sp - 1], m_mem[m_sp - 2]);
            return r;
        case Op::NEG:
            if (m_sp <= STACK_BASE) { r.ok = false; r.error = "Stack underflow (NEG)"; return r; }
            m_mem[m_sp - 1] = -m_mem[m_sp - 1];
            return r;
        case Op::OVER: {
            i32 top, second;
            if (!peek2(top, second)) { r.ok = false; r.error = "Stack underflow (OVER)"; return r; }
            if (!push(second)) { r.ok = false; r.error = "Stack overflow (OVER)"; }
            return r;
        }
        case Op::ADD:
        case Op::SUB:
        case Op::MUL:
        case Op::DIV:
        case Op::MOD: {
            if (!pop(b) || !pop(a)) { r.ok = false; r.error = "Stack underflow (arith)"; return r; }
            i32 res = 0;
            switch (op) {
                case Op::ADD: res = a + b; break;
                case Op::SUB: res = a - b; break;
                case Op::MUL: res = a * b; break;
                case Op::DIV:
                    if (b == 0) { r.ok = false; r.error = "Division by zero"; return r; }
                    res = a / b; break;
                case Op::MOD:
                    if (b == 0) { r.ok = false; r.error = "Modulo by zero"; return r; }
                    res = a % b; break;
                default: break;
            }
            if (!push(res)) { r.ok = false; r.error = "Stack overflow (arith)"; }
            return r;
        }
        case Op::CMP_EQ:
        case Op::CMP_LT:
        case Op::CMP_GT: {
            if (!pop(b) || !pop(a)) { r.ok = false; r.error = "Stack underflow (cmp)"; return r; }
            i32 res = 0;
            if (op == Op::CMP_EQ) res = (a == b) ? 1 : 0;
            else if (op == Op::CMP_LT) res = (a < b) ? 1 : 0;
            else if (op == Op::CMP_GT) res = (a > b) ? 1 : 0;
            if (!push(res)) { r.ok = false; r.error = "Stack overflow (cmp)"; }
            return r;
        }
        case Op::PRINT:
            if (!pop(a)) { r.ok = false; r.error = "Stack underflow (PRINT)"; return r; }
            std::printf("%d\n", a);
            return r;
        case Op::LOAD: {
            u32 addr;
            if (!fetchCell(addr)) { r.ok = false; r.error = "Truncated LOAD"; return r; }
            if (addr >= m_mem.size()) { r.ok = false; r.error = "LOAD out of range"; return r; }
            if (!push(m_mem[addr])) { r.ok = false; r.error = "Stack overflow (LOAD)"; }
            return r;
        }
        case Op::STORE: {
            u32 addr;
            if (!fetchCell(addr)) { r.ok = false; r.error = "Truncated STORE"; return r; }
            if (addr >= m_mem.size()) { r.ok = false; r.error = "STORE out of range"; return r; }
            if (!pop(a)) { r.ok = false; r.error = "Stack underflow (STORE)"; return r; }
            m_mem[addr] = a;
            return r;
        }
        case Op::STORE_IND: {
            i32 addrI32;
            if (!pop(addrI32)) { r.ok = false; r.error = "Stack underflow (STORE_IND addr)"; return r; }
            if (!pop(a)) { r.ok = false; r.error = "Stack underflow (STORE_IND value)"; return r; }
            const u32 addr = static_cast<u32>(addrI32);
            if (addr >= m_mem.size()) { r.ok = false; r.error = "STORE_IND out of range"; return r; }
            m_mem[addr] = a;
            return r;
        }
        case Op::JMP: {
            u32 addr;
            if (!fetchCell(addr)) { r.ok = false; r.error = "Truncated JMP"; return r; }
            if (addr >= MEM_SIZE) { r.ok = false; r.error = "JMP out of range"; return r; }
            m_ip = addr;
            return r;
        }
        case Op::JZ:
        case Op::JNZ: {
            u32 addr;
            if (!fetchCell(addr)) { r.ok = false; r.error = "Truncated JZ/JNZ"; return r; }
            if (!pop(a)) { r.ok = false; r.error = "Stack underflow (JZ/JNZ)"; return r; }
            bool cond = (a == 0);
            if ((op == Op::JZ && cond) || (op == Op::JNZ && !cond)) {
                if (addr >= MEM_SIZE) { r.ok = false; r.error = "Jump out of range"; return r; }
                m_ip = addr;
            }
            return r;
        }
        default:
            r.ok = false;
            r.error = "Invalid opcode";
            return r;
    }
}

} // namespace vm32
