/*
 * 6502SnakeBot - 6502 snake robot competition
 * File: Emulator.h
 *
 * Part of the 6502SnakeBot project (https://github.com/rainbain/6502SnakeBot)
 *
 * Copyright (c) 2026 Samuel Fitzsimons
 *
 * This file is licensed under the MIT License.
 * See the LICENSE file in the project root for full license terms.
 *
 * The 6502 Emulator.
 */

#pragma once

#include <vector>
#include <memory>
#include <stdint.h>

#include "mos6502.h"
#include "Game.h"

class Emulator {
    std::vector<uint8_t> RAM;   // 0000 - 3FFF
    std::vector<uint8_t> BOARD; // 4000 - Up to 4FFF
    std::vector<uint8_t> STATE; // 5000 - Up to 5FFF
    std::vector<uint8_t> ROM;   // 8000 - Up to FFEF
    std::vector<uint8_t> ISR;   // FFF0 - FFFF

    // Max clock cycles for a 6502 iterations.
    uint64_t max_clock_cycles;
public:
    Emulator(std::shared_ptr<Game> game, uint64_t max_clock_cycles);

    void load_rom(const std::string& path);

    // Runs a emulation round,
    // Returns false if game over
    bool cycle();

private:
    std::shared_ptr<Game> m_game;
    mos6502 core;

    void setup_state(const GameState& state);

    static void BusWrite(void* user, uint16_t addr, uint8_t value);
    static uint8_t BusRead(void* user, uint16_t addr);
};