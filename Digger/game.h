#pragma once
#include <SDL_ttf.h>
#include <SDL.h>
#include <vector>

#include "Map.h"
#include "Player.h"
#include "Enemy.h"

enum class GameState {
	PLAYING,
	GAME_OVER
};
class Game {
public:
	Game(SDL_Renderer* renderer);
	~Game();

	void handleEvents();
	void update();
	void render();
	void renderText(const char* text, int x, int y, SDL_Color color);
	void updateSpawner();

	bool isRunning()const;
private:
	SDL_Renderer* renderer = nullptr;
	bool running = true;

	Map map;
	Player player;
	std::vector<Enemy> enemies;

	int currentLevel = 0;
	int fade = 0;

	void loadLevel(int index);
	void checkCollisions();
	void renderGameOver();
	void renderHUD();
	
	GameState state = GameState::PLAYING;
	TTF_Font* font;

	int enemySpawnX = 0;
	int enemySpawnY = 0;
	int lastTimeSpawn = 0;
	int spawnDelay = 3000;
	int maxEnemies = 4;
};