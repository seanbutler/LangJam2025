# LangJam Roadmap

This is a lightweight list of follow-up ideas discussed while scaffolding the SDL + VM project.

## Near-term (next 1–3 sessions)

- Add `LOAD_IND` (indirect load) to complement `STORE_IND`.
  - Enables read/modify/write effects, sprite blits, and feedback.
- Add a tiny disassembler for `std::vector<u32>` programs.
  - Dump ops + immediates with addresses to help debug jumps.
- Add an execution trace mode (optional) for debugging.
  - Example: print `ip`, opcode, stack depth, and top few values.

## Graphics / “console” features

- Define a basic memory-mapped I/O spec (beyond the framebuffer):
  - Framebuffer: `VM::FB_BASE..FB_BASE+FB_SIZE` (ARGB8888)
  - Optional registers (ideas):
    - `FB_PRESENT` / `FB_DIRTY` flag
    - `DRAW_COLOR`
    - `CLIP_X0/Y0/X1/Y1`
- Add helper opcodes for graphics:
  - `PSET` (x, y, color)
  - `HLINE` / `VLINE`
  - `FILL` (fast clear)

## VM / ISA improvements

- Add bounds-checked memory helpers:
  - `MEMCPY`-like primitive for faster framebuffer uploads/blits.
- Add a call/return mechanism:
  - `CALL addr` / `RET` + a return stack (or reuse main stack with convention).
- Add bitwise ops:
  - `AND`, `OR`, `XOR`, `SHL`, `SHR` (useful for pixel math).

## Tooling and structure

- Move VM public headers into `include/` (optional) and keep sources in `vm/`.
- Add unit tests for the VM (single-step semantics for a few ops).
- Add a small “ROM” file format or loader (optional):
  - Store code cells and initial data cells.

## Performance notes

- Current SDL path copies `FB_SIZE` cells from VM memory each frame.
  - Future optimization: track a dirty flag/dirty rectangle to reduce copies.
  - Future optimization: point `SDL_UpdateTexture` directly at VM memory if the
    layout/aliasing constraints are acceptable.
