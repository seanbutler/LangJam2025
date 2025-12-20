
# LangJam Project Roadmap

## Next Steps

### Language

- [ ] AST
- [ ] Parser
- [ ] Symbol Table
- [ ] Scope?
- [ ] Code Generator



## Later Improvements

### Possible Graphics Instructions

  - [ ] ? `FB_PRESENT` / `FB_DIRTY` flag
  - [ ] `DRAW_COLOR`
  - [ ] `CLIP_X0/Y0/X1/Y1`
  - [ ] `PSET` (x, y, color)
  - [ ] `HLINE` / `VLINE`
  - [ ] `FILL` (fast clear)


### VM / ISA improvements

- [ ] `MEMCPY`- family of primitives for faster framebuffer uploads/blits.
- [ ] Call/return mechanism: `CALL addr` / `RET` + a return stack (or reuse main stack with convention).
- [ ] Bitwise ops `AND`, `OR`, `XOR`, `SHL`, `SHR` (useful for pixel math).
- [ ] Instructions for setting up memory layout, screen, stack etc


### Tooling and structure

- [ ] Move VM public headers into `include/` (optional) and keep sources in `vm/`.
- [ ] Add unit tests for the VM (single-step semantics for a few ops).
- [ ] Load PNGs into memory for graphics atlas etc
- [ ] Add a small “ROM” file format or loader (optional):
- [ ] Store code cells and initial data cells.


### Performance notes

- [ ] Current SDL path copies entire frame buffer each frame.
- [ ] Perhaps point `SDL_UpdateTexture` at VM memory directly rather than copy?
