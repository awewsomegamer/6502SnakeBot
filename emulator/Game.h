/*
 * 6502SnakeBot - 6502 snake robot competition
 * File: Game.h
 *
 * Part of the 6502SnakeBot project (https://github.com/rainbain/6502SnakeBot)
 *
 * Copyright (c) 2026 Samuel Fitzsimons
 *
 * This file is licensed under the MIT License.
 * See the LICENSE file in the project root for full license terms.
 *
 * Snake Game logic.
 */

#pragma once

#include <mutex>
#include <vector>
#include <random>

#include <stdint.h>

typedef struct {
    int x, y;
} GameVec2;

typedef enum {
    DIR_UP     = 1,
    DIR_DOWN   = 2,
    DIR_LEFT   = 3,
    DIR_RIGHT  = 4,
} Direction;

typedef struct {
    std::vector<GameVec2> snake;
    GameVec2 food;
    Direction snakeDirection;
    double score;
    double food_value;
    uint64_t total_ticks;
    uint64_t total_clock_cycles;
    uint64_t iteration_clock_cycles;
} GameState;

class Game {
    std::mutex state_lock;
    GameState state;
    std::mt19937 rng;

public:
    int board_width;
    int board_height;

    Game(int board_width, int board_height, unsigned int seed);

    void newState();
    GameState getState();
    void setState(const GameState& new_state);

    bool tick(Direction player_control);

private:
    void spawnFood(GameState& state);
    Direction flipDirection(Direction d);
};