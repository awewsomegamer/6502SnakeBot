# 6502SnakeBot
A 6502-based snake robot competition.

This repository hosts a competition to create the best snake bot in 6502 assembly with the smallest possible code size.

## Competition
The goal of this competition is to optimize two things:

### Code Size
The size of the ROM provided to the 6502 emulator, measured in bytes.

### Game Score
The score is the total number of food items collected by the snake.

**Time matters!** The longer food remains on the board without being eaten, the less it is worth.

### Game Parameters
The Snake game is run using the emulator’s default parameters:  
- **Board Width:** 17 tiles  
- **Board Height:** 17 tiles  
- **Max Cycles:** Unlimited for normal play  

> **Note:** The game seed is randomized for scoring, so your bot must handle different scenarios rather than relying on a fixed board layout.

### Scoring
For now, with few entries, scoring is mostly for bragging rights.  

If there are many entries, code size and game score will be normalized and combined to determine the final ranking.

## Project Status
This project currently has no entries—not even my own!  

I am currently testing the setup and building the first unoptimized bot.

---

## Getting Started

### Install Prerequisites
The emulator is a C++ program using SDL2.  
The 6502 code is assembled using CC65.

**Arch Linux / Manjaro:**
```bash
sudo pacman -S --needed base-devel gcc make sdl2 sdl2_ttf cc65
```

**Debian / Ubuntu:**
```bash
sudo apt update
sudo apt install build-essential g++ make libsdl2-dev libsdl2-ttf-dev cc65
```

### Clone the Repository
```bash
git clone https://github.com/rainbain/6502SnakeBot.git
```

### Build an Example
```bash
cd 6502SnakeBot/entries/golden/
make
```

### Build the Emulator
```bash
cd 6502SnakeBot/emulator/
make -j$(nproc)
```

### Run an Example
Run the example ROM:
```bash
./6502SnakeBot/emulator/emulator --rom 6502SnakeBot/entries/golden/golden.bin
```

To just play Snake:
```bash
./6502SnakeBot/emulator/emulator --play
```

To see more options:
```bash
./6502SnakeBot/emulator/emulator --help
```

---

### Coding Guide

For documentation on how the 6502 and its memory space are set up, see the hardware docs:  
[Hardware Documentation](hardware_docs.md)

For an example setup, check the example/template entry:  
[Golden Entry](entries/golden/README.md)

## Submitting Entries
To participate, fork the repository, develop your snake bot, and submit a pull request.

Make sure to read the **Code of Conduct** before submitting.

In the `entries/` folder, you can find a template entry named `golden`.  
A typical entry layout should look like this:
```
entries/
├─ ENTRY_NAME/
│  ├─ README.md   (REQUIRED)
│  ├─ LICENSE     (OPTIONAL)
│  ├─ code.asm    (REQUIRED)
│  ├─ Makefile    (REQUIRED)
```

### README.md
A required file with the name of your entry, description, and attribution.
You may include performance metrics (like in the golden entry), but this is optional.

### LICENSE
Optional. Include this only if you want to apply your own license in addition to the project’s MIT license.

### code.asm
Your source code, including any additional assembly files needed for your bot.  
File names may be anything, but the entry must assemble correctly with the Makefile.

### Makefile
A required Makefile to build your bot.  

**Requirements:**
- Must use **CA65** (from the CC65 toolchain) to assemble your code.  
- Must output a `.bin` file compatible with the emulator.  
- Follow the structure of the `golden` template to ensure consistent building and scoring.

---

## Code of Conduct
Code must be your own.

You may implement algorithms found online, but credit the original source in your `README.md`.

You may work in a team or get outside help, but all contributors must be credited in your `README.md`.