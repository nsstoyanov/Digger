#pragma once
#include "Map.h"
#include "SDL.h"
class Player
{
public:
	Player();
	void handleInput(const Uint8* keys,Map& map);
	void draw(SDL_Renderer* render) const;

	void update();

	int getTileX()const;
	int getTileY()const;

	void loseLife();
	bool isAlive()const;
	void respawn();
	int getLives()const;

	void setPossition(int x,int y);
	void setSpawn(int x, int y);


private:

	int tileX, tileY;
	int targetX, targetY;
	float pixelX, pixelY;
	int spawnX, spawnY;

	bool moving;
	Uint32 lastMoveTime;
	static constexpr Uint32 MOVE_DELAY = 150;
	static constexpr float MOVE_SPEED = 200.0f;

	int lives;
};

//tileX, tileY   - 	Game logic
//pixelX, pixelY  -	Rendering
//targetX, targetY  -	Movement control
//moving  -	 State
//lastMoveTime - Timing
//MOVE_DELAY  -	Game feel
//MOVE_SPEED  -	Visual smoothness