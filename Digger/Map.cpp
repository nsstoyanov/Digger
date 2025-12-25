#include "Map.h"


Map::Map()
{
	init();
}

void Map::init()
{
	for (int x = 0; x < MAP_WIDTH; x++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			tiles[x][y] = Tile::DIRT;
		}
	}
}

void Map::draw(SDL_Renderer* render) const
{
	SDL_Rect rect;
	rect.w = TILE_SIZE;
	rect.h = TILE_SIZE;
	for (int x = 0; x < MAP_WIDTH; x++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			switch (tiles[x][y]) {
			case Tile::DIRT: SDL_SetRenderDrawColor(render, 139, 69, 19, 255);
				break;
			case Tile::EMPTY: SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
				break;
			case Tile::WALL: SDL_SetRenderDrawColor(render, 120, 120, 120, 255);
				break;
			}
			rect.x = x * TILE_SIZE;
			rect.y = y * TILE_SIZE;
			SDL_RenderFillRect(render, &rect);
		}
	}
}

bool Map::isInside(int x, int y) const
{
	return (x >= 0 && x < MAP_WIDTH) && (y >= 0 && y < MAP_HEIGHT);
}

bool Map::isWalkable(int x, int y) const
{
	if (!isInside(x, y)) 
		return false;
	
	return tiles[x][y] != Tile::WALL;
}

void Map::dig(int x, int y)
{
	if (!isInside(x, y))
		return;
	if (tiles[x][y] == Tile::DIRT) {
		tiles[x][y] = Tile::EMPTY;
	}

;}
bool Map:: isTunnel(int x, int y) const {
	if (!isInside(x, y))
		return false;
	return tiles[x][y] == Tile::EMPTY;

}

void Map::load(const char level[MAP_HEIGHT][MAP_WIDTH + 1])
{
	enemySpawns.clear();
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			switch (level[y][x])
			{
			case '#':
				tiles[x][y] = Tile::WALL;
				break;

			case '|':
				tiles[x][y] = Tile::EMPTY;
				break;

			case 'P':
				playerSpawnX = x;
				playerSpawnY = y;
				tiles[x][y] = Tile::EMPTY;
				break;

			case 'E':
				enemySpawns.push_back({ x,y });
				tiles[x][y] = Tile::EMPTY;
				break;

			default:
				tiles[x][y] = Tile::DIRT;
				break;
			}
		}
	}
}
int Map::getPlayerSpawnX() const {
	return playerSpawnX;
}
int Map::getPlayerSpawnY() const {
	return playerSpawnY;
}

const vector<SpawnPoint> Map::getEnemySpawns() const
{
	return enemySpawns;
}

