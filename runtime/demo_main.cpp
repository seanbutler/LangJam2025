#include "vm.h"
#include "bytecode_builder.h"
#include <cstdio>

using namespace vm32;

int main() {
    BytecodeBuilder c;
    // Use unified memory regions: DATA_BASE for variables
    const u32 ACC = vm32::VM::DATA_BASE + 0;
    const u32 I   = vm32::VM::DATA_BASE + 1;

    // init
    c.pushi(1); c.store(ACC);
    c.pushi(5); c.store(I);

    auto LOOP = (u32)c.pc();
    // if (i==0) goto END
    c.load(I);      // i
    c.pushi(0);
    c.cmpeq();
    c.jnz(0);      // patch later
    auto jnz_end_patch = (u32)c.pc() - 1; // index of JNZ target cell

    // acc = acc * i
    c.load(ACC);
    c.load(I);
    c.mul();
    c.store(ACC);

    // i = i - 1
    c.load(I);
    c.pushi(1);
    c.sub();
    c.store(I);

    // goto LOOP
    c.jmp(LOOP);

    auto END = (u32)c.pc();
    c.load(ACC);
    c.print();
    c.halt();

    // Patch END address (cells, not bytes)
    c.code[jnz_end_patch] = END;

    VM vm;
    vm.load(c.code);
    auto res = vm.run();
    if (!res.ok) {
        std::fprintf(stderr, "VM error: %s\n", res.error.c_str());
        return 1;
    }
    return 0;
}
