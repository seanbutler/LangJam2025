#pragma once
#include "vm.h"
#include <vector>

namespace vm32 {

struct BytecodeBuilder {
    std::vector<u32> code;

    static void emitU32(std::vector<u32>& c, u32 v) { c.push_back(v); }
    static void emitU8(std::vector<u32>& c, u8 v) { c.push_back(static_cast<u32>(v)); }

    std::size_t pc() const { return code.size(); }

    BytecodeBuilder& op(Op o) { code.push_back(static_cast<u32>(o)); return *this; }
    BytecodeBuilder& pushi(i32 v) { op(Op::PUSHI); emitU32(code, static_cast<u32>(v)); return *this; }
    BytecodeBuilder& pop() { return op(Op::POP); }
    BytecodeBuilder& dup() { return op(Op::DUP); }
    BytecodeBuilder& swap(){ return op(Op::SWAP); }
    BytecodeBuilder& over(){ return op(Op::OVER); }
    BytecodeBuilder& neg() { return op(Op::NEG); }

    BytecodeBuilder& add() { return op(Op::ADD); }
    BytecodeBuilder& sub() { return op(Op::SUB); }
    BytecodeBuilder& mul() { return op(Op::MUL); }
    BytecodeBuilder& div() { return op(Op::DIV); }
    BytecodeBuilder& mod() { return op(Op::MOD); }

    BytecodeBuilder& print() { return op(Op::PRINT); }

    BytecodeBuilder& jmp(u32 addr) { op(Op::JMP); emitU32(code, addr); return *this; }
    BytecodeBuilder& jz(u32 addr)  { op(Op::JZ);  emitU32(code, addr); return *this; }
    BytecodeBuilder& jnz(u32 addr) { op(Op::JNZ); emitU32(code, addr); return *this; }

    BytecodeBuilder& cmpeq(){ return op(Op::CMP_EQ); }
    BytecodeBuilder& cmplt(){ return op(Op::CMP_LT); }
    BytecodeBuilder& cmpgt(){ return op(Op::CMP_GT); }

    BytecodeBuilder& halt() { return op(Op::HALT); }

    BytecodeBuilder& load(u32 addr)  { op(Op::LOAD); emitU32(code, addr); return *this; }
    BytecodeBuilder& store(u32 addr) { op(Op::STORE); emitU32(code, addr); return *this; }

    BytecodeBuilder& store_ind() { return op(Op::STORE_IND); }
};

} // namespace vm32
