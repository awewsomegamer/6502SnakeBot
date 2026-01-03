#include "GameRender.h"

#include <stdexcept>
#include <format>

#define TEXT_MARGIN 0.2
#define BOARD_MARGIN 0.01
#define FOOD_MARGIN 0.2
#define SNAKE_BODY_MARGIN 0.2

#define FONT_SIZE 20

GameRender::GameRender(std::shared_ptr<Game> game) : m_game(game) {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    if(TTF_Init() != 0) {
        throw std::runtime_error(SDL_GetError());
    }

    SDL_Point window_size = this->getDefaultWindowSize();

    // Create window
    window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_size.x, window_size.y,
        SDL_WINDOW_SHOWN
    );

    if(!window) {
        throw std::runtime_error(SDL_GetError());
    }

    // Create Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(!renderer) {
        throw std::runtime_error(SDL_GetError());
    }

    // Get Asset Directory
    char* base_path = SDL_GetBasePath();
    if (!base_path) {
        throw std::runtime_error(SDL_GetError());
    }

    std::string font_path = std::string(base_path) + "DejaVuSans.ttf";
    this->font = TTF_OpenFont(font_path.c_str(), FONT_SIZE);
    if (!font) {
        throw std::runtime_error(TTF_GetError());
    }
}

GameRender::~GameRender() {
    for(size_t i = 0; i < texcahche.size(); i++) {
        if(texcahche[i].tex == NULL) {
            SDL_DestroyTexture(texcahche[i].tex);
        }
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

SDL_Point GameRender::getDefaultWindowSize() {
    int board_width = 32 * m_game->board_width;
    int board_height = 32 * m_game->board_height;

    board_width += (int)(TEXT_MARGIN * board_width);

    return (SDL_Point){board_width, board_height};
}

SDL_Rect GameRender::getGameArena() {
    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(this->window, &window_width, &window_height);

    int text_margin_size_x = window_width * TEXT_MARGIN;
    int board_margin_size_x = window_width * BOARD_MARGIN;
    int board_margin_size_y = window_height * BOARD_MARGIN;

    return SDL_Rect {
        .x = text_margin_size_x+board_margin_size_x, .y = board_margin_size_y,
        .w = window_width - text_margin_size_x - 2 * board_margin_size_x, .h = window_height - 2 * board_margin_size_y
    };
}

SDL_Rect GameRender::getInfoMargin() {
    int window_width = 0, window_height = 0;
    SDL_GetWindowSize(this->window, &window_width, &window_height);

    int text_margin_size_x = window_width * TEXT_MARGIN;
    int board_margin_size_x = window_width * BOARD_MARGIN;
    int board_margin_size_y = window_height * BOARD_MARGIN;

    return SDL_Rect {
        .x = board_margin_size_x, .y = board_margin_size_y,
        .w = text_margin_size_x, .h = window_height - 2 * board_margin_size_y
    };
}

void GameRender::render(bool game_over) {
    // Clear Window
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // Draw game
    SDL_Rect arena = getGameArena();
    SDL_Rect info = getInfoMargin();

    draw_board(arena, info, game_over);

    // Show Window
    SDL_RenderPresent(renderer);
}

SDL_Rect GameRender::getTile(SDL_Rect arena, GameVec2 pos) {
    int tile_size_x = arena.w / m_game->board_width;
    int tile_size_y = arena.h / m_game->board_height;

    SDL_Rect tile_rect = {
        .x = pos.x * tile_size_x + arena.x + 1,
        .y = pos.y * tile_size_y + arena.y + 1,
        .w = tile_size_x - 1,
        .h = tile_size_y - 1
    };

    return tile_rect;
}

void GameRender::draw_board(SDL_Rect arena, SDL_Rect info, bool game_over) {
    // Get the game state
    const GameState state = m_game->getState();

    // Draw Grid Using Lines
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);

    int tile_size_x = arena.w / m_game->board_width;
    int tile_size_y = arena.h / m_game->board_height;
    
    // Vertical Grid Lines
    for(int x = 0; x <= m_game->board_width; x++) {
        int xp = x * tile_size_x + arena.x;
        SDL_RenderDrawLine(
            this->renderer, xp, arena.y, xp, arena.y + m_game->board_height * tile_size_y
        );
    }

    // Horizontal Grid Lines
    for(int y = 0; y <= m_game->board_height; y ++) {
        int yp = y * tile_size_y + arena.y;
        SDL_RenderDrawLine(
            this->renderer, arena.x, yp, arena.x + m_game->board_width * tile_size_x, yp
        );
    }

    // Draw Food
    draw_food(getTile(arena, state.food));

    // Create body
    std::vector<int> body;
    body.resize(state.snake.size());

    body[0] = 0;
    for(size_t i = 1; i < body.size(); i++) {
        int dx = state.snake[i].x - state.snake[i-1].x;
        int dy = state.snake[i].y - state.snake[i-1].y;

        if(dx < 0) {
            body[i] = RIGHT;
            body[i-1] |= LEFT;
        }
        if(dx > 0) {
            body[i] = LEFT;
            body[i-1] |= RIGHT;
        }
        if(dy < 0) {
            body[i] = DOWN;
            body[i-1] |= UP;
        }
        if(dy > 0) {
            body[i] = UP;
            body[i-1] |= DOWN;
        }
    }

    for(size_t i = 0; i < body.size(); i++) {
        draw_body(getTile(arena, state.snake[i]), body[i]);
    }

    draw_text(0, "Score:", (SDL_Point){info.x, info.y});
    draw_text(1, std::format("{:.3f}", state.score).c_str(), (SDL_Point){info.x, info.y+FONT_SIZE});
    
    draw_text(2, "Value:", (SDL_Point){info.x, info.y+FONT_SIZE*3});
    draw_text(3, std::format("{:.3f}", state.food_value).c_str(), (SDL_Point){info.x, info.y+FONT_SIZE*4});

    draw_text(4, "Ticks:", (SDL_Point){info.x, info.y+FONT_SIZE*6});
    draw_text(5, std::format("{}", state.total_ticks).c_str(), (SDL_Point){info.x, info.y+FONT_SIZE*7});

    draw_text(6, "Cycles:", (SDL_Point){info.x, info.y+FONT_SIZE*9});
    draw_text(7, std::format("{}", state.iteration_clock_cycles).c_str(), (SDL_Point){info.x, info.y+FONT_SIZE*10});

    draw_text(8, "Total Cycles:", (SDL_Point){info.x, info.y+FONT_SIZE*12});
    draw_text(9, std::format("{}", state.total_clock_cycles).c_str(), (SDL_Point){info.x, info.y+FONT_SIZE*13});

    if(game_over) {
        draw_text(10, "Game Over", (SDL_Point){info.x, info.y+FONT_SIZE*15});
    }
}

void GameRender::draw_food(SDL_Rect tile) {
    SDL_SetRenderDrawColor(renderer, 250, 10, 10, 255);

    int margin_width = (int)(tile.w * FOOD_MARGIN);
    int margin_height = (int)(tile.h * FOOD_MARGIN);

    tile.x += margin_width;
    tile.y += margin_height;
    tile.w -= margin_width * 2;
    tile.h -= margin_height * 2;

    SDL_RenderFillRect(this->renderer, &tile);
}

void GameRender::draw_body(SDL_Rect tile, int type) {
    SDL_SetRenderDrawColor(renderer, 10, 250, 10, 255);

    int margin_width = (int)(tile.w * SNAKE_BODY_MARGIN);
    int margin_height = (int)(tile.h * SNAKE_BODY_MARGIN);

    // Draw core
    {
        SDL_Rect r = {
            .x = margin_width, .y = margin_height,
            .w = tile.w - 2 * margin_width, .h = tile.h - 2 * margin_height
        };

        r.x += tile.x;
        r.y += tile.y;
        SDL_RenderFillRect(this->renderer, &r);
    }

    if(type & LEFT) {
        SDL_Rect r = {
            .x = 0, .y = margin_height,
            .w = margin_width, .h = tile.h - 2 * margin_height
        };

        r.x += tile.x;
        r.y += tile.y;
        SDL_RenderFillRect(this->renderer, &r);
    }

    if(type & RIGHT) {
        SDL_Rect r = {
            .x = tile.w - 2 * margin_width, .y = margin_height,
            .w = tile.w, .h = tile.h - 2 * margin_height
        };

        r.x += tile.x;
        r.y += tile.y;
        SDL_RenderFillRect(this->renderer, &r);
    }

    if(type & UP) {
        SDL_Rect r = {
            .x = margin_width, .y = 0,
            .w = tile.w - 2 * margin_width, .h = margin_height
        };

        r.x += tile.x;
        r.y += tile.y;
        SDL_RenderFillRect(this->renderer, &r);
    }

    if(type & DOWN) {
        SDL_Rect r = {
            .x = margin_width, .y = tile.h - 2 * margin_height,
            .w = tile.w - 2 * margin_width, .h = tile.h
        };

        r.x += tile.x;
        r.y += tile.y;
        SDL_RenderFillRect(this->renderer, &r);
    }
}

void GameRender::draw_text(size_t cache_index, std::string text, SDL_Point pos) {    
    if(cache_index >= texcahche.size()) {
        texcahche.resize(cache_index + 1);
        texcahche[cache_index].tex = NULL;
    }
    TextLabel& cache = texcahche[cache_index];
    
    // If cache busted, recache
    if(text != cache.text || cache.tex == NULL) {
        if(cache.tex != NULL) {
            SDL_DestroyTexture(cache.tex);
        }
        cache.text = text;

        SDL_Color white = {250, 250, 250, 255};
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), white);
        cache.tex =  SDL_CreateTextureFromSurface(renderer, surface);
        
        SDL_QueryTexture(cache.tex, NULL, NULL, &cache.rect.w, &cache.rect.h);

        // Trans Rights
        SDL_FreeSurface(surface);
    }

    cache.rect.x = pos.x;
    cache.rect.y = pos.y;
    SDL_RenderCopy(renderer, cache.tex, NULL, &cache.rect);
}