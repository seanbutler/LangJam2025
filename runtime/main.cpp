#include <SDL.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "vm.h"
#include "bytecode_builder.h"

int main(int argc, char* argv[]) {

    std::cout << "runtime " << std::endl;


    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    std::printf("Hello SDL\n");

    
    SDL_Window* window = SDL_CreateWindow(
        "LangJam SDL", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        256, 192, 
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Start at a chunky 4x scale (1024x768) while keeping a logical 256x192 resolution.
    SDL_SetWindowSize(window, vm32::VM::FB_WIDTH * 4, vm32::VM::FB_HEIGHT * 4);

    SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    
    if (!renderer) {
        std::printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (SDL_RenderSetLogicalSize(renderer, vm32::VM::FB_WIDTH, vm32::VM::FB_HEIGHT) != 0) {
        std::printf("SDL_RenderSetLogicalSize Error: %s\n", SDL_GetError());
    }

    if (SDL_RenderSetIntegerScale(renderer, SDL_TRUE) != 0) {
        std::printf("SDL_RenderSetIntegerScale Error: %s\n", SDL_GetError());
    }

    // Create a 32-bit framebuffer texture (ARGB8888) backed by VM memory
    SDL_Texture* framebufferTex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        vm32::VM::FB_WIDTH,
        vm32::VM::FB_HEIGHT
    );

    if (!framebufferTex) {
        std::printf("SDL_CreateTexture Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    vm32::VM vm;

    // VM drawing demo: fill the memory-mapped framebuffer using a loop.
    // Any STORE/STORE_IND into [VM::FB_BASE, VM::FB_BASE + VM::FB_SIZE) becomes a pixel.
    {
        using namespace vm32;
        BytecodeBuilder bc;

        // Variables in data memory
        const u32 I    = VM::DATA_BASE + 0; // loop counter
        const u32 ADDR = VM::DATA_BASE + 1; // computed framebuffer address

        const u32 FB_N = VM::FB_SIZE;

        // i = 0
        bc.pushi(0).store(I);

        const u32 LOOP = static_cast<u32>(bc.pc());

        // if (i < FB_SIZE) continue; else goto END
        bc.load(I).pushi(static_cast<i32>(FB_N)).cmplt();
        bc.jz(0);
        const u32 jz_end_patch = static_cast<u32>(bc.pc()) - 1; // target cell index

        // addr = FB_BASE + i
        bc.pushi(static_cast<i32>(VM::FB_BASE)).load(I).add().store(ADDR);

        // Checker/stripe-ish pattern: ((i % 32) < 16) ? color1 : color2
        bc.load(I).pushi(32).mod().pushi(16).cmplt();
        bc.jz(0);
        const u32 jz_else_patch = static_cast<u32>(bc.pc()) - 1; // target cell index

        // THEN: color1
        bc.pushi(static_cast<i32>(0xFF33AAFFu));
        bc.load(ADDR);
        bc.store_ind();
        bc.jmp(0);
        const u32 jmp_inc_patch = static_cast<u32>(bc.pc()) - 1; // target cell index

        // ELSE:
        const u32 ELSE = static_cast<u32>(bc.pc());
        bc.pushi(static_cast<i32>(0xFFFF2244u));
        bc.load(ADDR);
        bc.store_ind();

        // INC:
        const u32 INC = static_cast<u32>(bc.pc());
        bc.load(I).pushi(1).add().store(I);
        bc.jmp(LOOP);

        const u32 END = static_cast<u32>(bc.pc());
        bc.halt();

        // Patch jump targets
        bc.code[jz_end_patch] = END;
        bc.code[jz_else_patch] = ELSE;
        bc.code[jmp_inc_patch] = INC;

        vm.load(bc.code);

        auto r = vm.run(5'000'000);
        
        if (!r.ok) {
            std::printf("VM error: %s\n", r.error.c_str());
        }
    }

    std::vector<std::uint32_t> pixels(vm32::VM::FB_SIZE);

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        // Update memory-mapped keyboard/gamepad state.
        // VM bytecode can read this with: LOAD VM::KB_STATE_ADDR
        SDL_PumpEvents();
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        vm32::u32 kb = 0;
        if (keys[SDL_SCANCODE_UP])    kb |= vm32::VM::KB_UP;
        if (keys[SDL_SCANCODE_DOWN])  kb |= vm32::VM::KB_DOWN;
        if (keys[SDL_SCANCODE_LEFT])  kb |= vm32::VM::KB_LEFT;
        if (keys[SDL_SCANCODE_RIGHT]) kb |= vm32::VM::KB_RIGHT;

        // Typical retro mappings
        if (keys[SDL_SCANCODE_Z])      kb |= vm32::VM::KB_A;
        if (keys[SDL_SCANCODE_X])      kb |= vm32::VM::KB_B;
        if (keys[SDL_SCANCODE_A])      kb |= vm32::VM::KB_X;
        if (keys[SDL_SCANCODE_S])      kb |= vm32::VM::KB_Y;
        if (keys[SDL_SCANCODE_Q])      kb |= vm32::VM::KB_L;
        if (keys[SDL_SCANCODE_E])      kb |= vm32::VM::KB_R;
        if (keys[SDL_SCANCODE_RSHIFT]) kb |= vm32::VM::KB_SELECT;
        if (keys[SDL_SCANCODE_RETURN]) kb |= vm32::VM::KB_START;

        vm.setKeyboardState(kb);

        // Copy VM framebuffer memory into a texture and display it.
        for (std::size_t i = 0; i < pixels.size(); ++i) {
            const vm32::u32 addr = vm32::VM::FB_BASE + static_cast<vm32::u32>(i);
            pixels[i] = static_cast<std::uint32_t>(vm.memAt(addr));
        }

        SDL_UpdateTexture(
            framebufferTex,
            nullptr,
            pixels.data(),
            static_cast<int>(vm32::VM::FB_WIDTH * sizeof(std::uint32_t))
        );

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, framebufferTex, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(framebufferTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
