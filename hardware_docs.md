# 6502SnakeBot Hardware Docs

## Operation
The bot interacts with the snake game through memory-mapped registers: it reads the `BOARD` and writes to `STATE` to control the snake.

Each game step proceeds as follows:
1. The emulator asserts an interrupt request (`IRQ`).
2. Your program acknowledges the interrupt, calculates the next move into `Snake Direction`, then sets the `DONE` bit of the `CONTROL` register.
3. The emulator moves the snake in the direction specified.

---

## Memory Map
| Address Range | Segment | Access |
|---------------|---------|--------|
| 0x0000–0x3FFF | RAM     | R/W    |
| 0x4000–0x4FFF | BOARD   | R/O    |
| 0x5000–0x5FFF | STATE   | R/W    |
| 0x8000–0xFFEF | ROM     | R/O    |
| 0xFFF0–0xFFFF | ISR     | R/W    |

- The ROM is loaded at `0x8000`.
- Execution begins at `0x8000`.

---

### RAM (0x0000–0x3FFF)
16 KB of general-purpose random access memory.

---

### BOARD (0x4000–0x4FFF)
This contains the snake game board as tiles, arranged in **rows** and **columns**.  

- Each byte represents a tile:  
  - **Upper nibble (bits 7–4):** tile type  
  - **Lower nibble (bits 3–0):** direction  

By default, the board is **17 rows × 17 columns**, occupying 289 bytes.

#### Tile Bitfield
| Bits | Field      | Values |
|------|-----------|--------|
| 7–4  | TILE_TYPE | EMPTY (0), SNAKE_HEAD (1), SNAKE_BODY (2), SNAKE_TAIL (3), FOOD (4) |
| 3    | DIRECTION | DOWN   |
| 2    | DIRECTION | UP     |
| 1    | DIRECTION | RIGHT  |
| 0    | DIRECTION | LEFT   |

> Only one direction bit should be set at a time.

---

### STATE (0x5000–0x5FFF)
Contains the game state.

| Address | Byte | Register         |
|---------|------|-----------------|
| 0x5000  | 0    | CONTROL          |
| 0x5001  | 1    | Snake Direction  |

#### CONTROL Register
| Bit | Name         | Access | Function |
|-----|-------------|--------|----------|
| 0   | IRQ_ASSERTED | R/W    | Set to 0 to acknowledge interrupt |
| 1   | DONE         | W      | Set to 1 when move is calculated; auto-clears |

#### Snake Direction Register
| Bit | Direction |
|-----|-----------|
| 3   | DOWN      |
| 2   | UP        |
| 1   | RIGHT     |
| 0   | LEFT      |

---

### ROM (0x8000–0xFFEF)
Contains the main program code. Read-only.

---

### ISR (0xFFF0–0xFFFF)
Interrupt vector table. Each vector contains the address of the corresponding interrupt service routine.

| Address | Bytes | Vector | Default |
|--------:|-------|--------|---------|
| 0xFFF0  | 2     | Reserved | 0x8000 |
| 0xFFF2  | 2     | Reserved | 0x8000 |
| 0xFFF4  | 2     | Reserved | 0x8000 |
| 0xFFF6  | 2     | Reserved | 0x8000 |
| 0xFFF8  | 2     | Reserved | 0x8000 |
| 0xFFFA  | 2     | NMI      | 0x8000 |
| 0xFFFC  | 2     | RESET    | 0x8000 |
| 0xFFFE  | 2     | IRQ/BRK  | 0x8000 |

> Writing to these addresses overrides the default vector.

