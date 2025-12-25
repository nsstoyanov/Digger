#include <SDL.h>
#include "Game.h"
#include <SDL_ttf.h>


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();


    SDL_Window* window = SDL_CreateWindow(
        "Player Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        MAP_WIDTH * TILE_SIZE,
        MAP_HEIGHT * TILE_SIZE,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Game game(renderer);

    while (game.isRunning())
    {
        game.handleEvents();
        game.update();
        game.render();

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
