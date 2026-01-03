#pragma once

#include <memory>
#include <string>

#include <SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Game.h"

class GameRender {
public:
    GameRender(std::shared_ptr<Game> game);
    ~GameRender();

    void render(bool game_over);

    SDL_Point getDefaultWindowSize();
    SDL_Rect getGameArena();
    SDL_Rect getInfoMargin();

private:
    enum BodyType {
        NONE = 0,
        LEFT  = 0b0001,
        RIGHT = 0b0010,
        UP    = 0b0100,
        DOWN  = 0b1000,
        ALL   = 0b1111
    };

    typedef struct {
        SDL_Texture* tex;
        SDL_Rect rect;
        std::string text;
    } TextLabel;

    std::vector<TextLabel> texcahche;

    std::shared_ptr<Game> m_game;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    SDL_Rect getTile(SDL_Rect arena, GameVec2 pos);

    void draw_board(SDL_Rect arena, SDL_Rect info, bool game_over);
    void draw_food(SDL_Rect tile);
    void draw_body(SDL_Rect tile, int type);
    void draw_text(size_t cache_index, std::string text, SDL_Point pos);
};