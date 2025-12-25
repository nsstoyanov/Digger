#include "Game.h"
#include <iostream>
#include "Level.h"
#include <cstring>
#include <string>

Game::Game(SDL_Renderer* render) : renderer(render){
    font = TTF_OpenFont("C:/Users/Nikola/Documents/SD/Digger/x64/Debug/assets/Roboto_Condensed-Bold.ttf", 32);
 
    if (!font)
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            "FONT ERROR",
            TTF_GetError(),
            nullptr
        );
    }
    loadLevel(currentLevel);
}
Game::~Game()
{
    if (font)
        TTF_CloseFont(font);
}
bool Game::isRunning()const {
	return running;
}
void Game::loadLevel(int index)
{
    enemies.clear();

    map.load(getLevel(currentLevel));

    player.setSpawn(
        map.getPlayerSpawnX(),
        map.getPlayerSpawnY()
    );
    player.respawn();
    const auto& spawns = map.getEnemySpawns();
    if (!spawns.empty()) {
        enemySpawnX = spawns[0].x;
        enemySpawnY = spawns[0].y;

        enemies.emplace_back(enemySpawnX, enemySpawnY);
    }
    lastTimeSpawn = SDL_GetTicks();
}
void Game::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            running = false;

        if (state == GameState::GAME_OVER &&
            event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_r)
            {
                state = GameState::PLAYING;
                loadLevel(currentLevel);
            }
            else if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }
    }
}
void Game::update()
{
    if (state != GameState::PLAYING)
        return;

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    player.handleInput(keys, map);

    player.update();
    updateSpawner();

    for (Enemy& enemy : enemies)
    {
        enemy.update(map, player.getTileX(), player.getTileY());
    }

    checkCollisions();

}
void Game::checkCollisions()
{
    for (Enemy& e : enemies)
    {
        if (
            player.getTileX() == e.getTileX() &&
            player.getTileY() == e.getTileY())
        {
            player.loseLife();

            if (!player.isAlive())
            {
                state = GameState::GAME_OVER;
                SDL_Log("GAME OVER STATE SET");

                fade = 0;
                return;
            }

            for (Enemy& resetEnemy : enemies)
                resetEnemy.reset();

            return;
        }
    }
}
void Game::renderGameOver()
{
    if (fade < 180)
        fade += 4;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, fade);
    SDL_Rect overlay{
        0, 0,
        MAP_WIDTH * TILE_SIZE,
        MAP_HEIGHT * TILE_SIZE
    };
  
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_Color red{ 255, 0, 0 };
    SDL_Color white{ 255, 255, 255 };

    renderText("GAME OVER",
        MAP_WIDTH * TILE_SIZE / 2,
        MAP_HEIGHT * TILE_SIZE / 2 - 40,
        red);

    renderText("Press R to Restart",
        MAP_WIDTH * TILE_SIZE / 2,
        MAP_HEIGHT * TILE_SIZE / 2 + 10,
        white);

    renderText("Press ESC to Quit",
        MAP_WIDTH * TILE_SIZE / 2,
        MAP_HEIGHT * TILE_SIZE / 2 + 40,
        white);
}
void Game::renderHUD()
{
    SDL_Color white{ 255, 255, 255 };

    std::string livesText = "Lives: " + std::to_string(player.getLives());

    renderText(
        livesText.c_str(),
        60,   // x
        17,   // y
        white
    );

}
void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
   

    map.draw(renderer);
    player.draw(renderer);

    for (Enemy& enemy : enemies)
    {
        enemy.draw(renderer);
    }
    if (state == GameState::PLAYING)
    {
        renderHUD();
    }
    if (state == GameState::GAME_OVER)
    {
        renderGameOver();
    }

    SDL_RenderPresent(renderer);
}
void Game::renderText(const char* text, int x, int y, SDL_Color color)
{
    if (!font) return;

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        SDL_Log("TTF_RenderText_Solid failed: %s", TTF_GetError());
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst;
    dst.w = surface->w;
    dst.h = surface->h;
    dst.x = x - dst.w / 2;
    dst.y = y - dst.h / 2;

    SDL_FreeSurface(surface);

    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}

void Game::updateSpawner()
{
    if (enemies.size() >= maxEnemies)
        return;
    int now = SDL_GetTicks();
    if (now - lastTimeSpawn >= spawnDelay) {
        enemies.emplace_back(enemySpawnX, enemySpawnY);
        lastTimeSpawn = now;
    }
}
