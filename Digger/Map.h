#pragma once
#include "SDL.h"
#include <vector>
using namespace std;
constexpr int TILE_SIZE = 32;
constexpr int MAP_WIDTH = 20;
constexpr int MAP_HEIGHT = 15;
enum class Tile {
	EMPTY,
	DIRT,
	WALL
};
struct SpawnPoint
{
	int x;
	int y;
};
class Map
{
public:
	Map();
	void init();
	void draw(SDL_Renderer* render) const;

	bool isInside(int x, int y) const;
	bool isWalkable(int x, int y) const;
	void dig(int x, int y);
	bool isTunnel(int x, int y) const;
	void load(const char level[MAP_HEIGHT][MAP_WIDTH + 1]);

	int getPlayerSpawnX() const;
	int getPlayerSpawnY() const;

    const vector<SpawnPoint> getEnemySpawns() const;

private:
	Tile tiles[MAP_WIDTH][MAP_HEIGHT];

	int playerSpawnX = 0;
	int playerSpawnY = 0;

	vector<SpawnPoint> enemySpawns;
};

