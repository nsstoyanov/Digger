#pragma once
#include "Map.h"
#include "SDL.h"
class Enemy {
public:
	Enemy();
	Enemy(int spawnX, int spawnY);
	void update(Map& map, int playerX, int playerY);
	void draw(SDL_Renderer* render) const;
	int getTileX()const;
	int getTileY()const;
	void reset();
	void setPossition(int x, int y);
	void setSpawn(int x, int y);

private:

	int tileX, tileY;
	int targetX, targetY;
	float pixelX, pixelY;
	int spawnX, spawnY;
	int lastDirX = 0;
	int lastDirY = 0;

	bool moving;
	Uint32 lastMoveTime;
	static constexpr Uint32 MOVE_DELAY = 300;
	static constexpr float MOVE_SPEED = 200.0f;
};
