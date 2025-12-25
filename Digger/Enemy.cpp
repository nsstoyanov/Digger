#include "Enemy.h"
#include <cstdlib>
int manhattan(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

Enemy::Enemy()
{
	tileX = 0;
	tileY = 0;

	pixelX = 0.0f;
	pixelY = 0.0f;

	targetX = 0;
	targetY = 0;

	moving = false;
	lastMoveTime = 0;

	spawnX = 0;
	spawnY = 0;
}

Enemy::Enemy(int spawnX, int spawnY) : Enemy()
{
	setSpawn(spawnX, spawnY);
}

void Enemy::update(Map& map, int playerX, int playerY)
{
	Uint32 now = SDL_GetTicks();

	if (moving) {
		float tx = targetX * TILE_SIZE;
		float ty = targetY * TILE_SIZE;
		float dx = tx - pixelX;
		float dy = ty - pixelY;

		float distance = SDL_sqrt(dx * dx + dy * dy);

		float step = MOVE_SPEED * (1.0f / 60.0f);
		if (distance <= step)
		{
			pixelX = tx;
			pixelY = ty;

			tileX = targetX;
			tileY = targetY;

			moving = false;
		}
		else
		{
			pixelX += dx / distance * step;
			pixelY += dy / distance * step;
		}
		return;

	}
	if (now - lastMoveTime < MOVE_DELAY)
		return;
	lastMoveTime = now;

	const int dirs[4][2] = {
		{ 1, 0 },  // right
		{ -1, 0 }, // left
		{ 0, 1 },  // down
		{ 0, -1 }  // up
	};

	int bestDist = 999999;
	std::vector<std::pair<int, int>> bestMoves;

	for (int i = 0; i < 4; i++)
	{
		int dx = dirs[i][0];
		int dy = dirs[i][1];

		if (dx == -lastDirX && dy == -lastDirY)
			continue;

		int nx = tileX + dx;
		int ny = tileY + dy;

		if (!map.isTunnel(nx, ny))
			continue;

		int d = manhattan(nx, ny, playerX, playerY);

		if (d < bestDist)
		{
			bestDist = d;
			bestMoves.clear();
			bestMoves.push_back({ dx, dy });
		}
		else if (d == bestDist)
		{
			bestMoves.push_back({ dx, dy });
		}
	}

	if (bestMoves.empty())
	{
		int rx = -lastDirX;
		int ry = -lastDirY;
		int nx = tileX + rx;
		int ny = tileY + ry;

		if (map.isTunnel(nx, ny))
		{
			targetX = nx;
			targetY = ny;
			lastDirX = rx;
			lastDirY = ry;
			moving = true;
			return;
		}

		return;
	}

	auto choice = bestMoves[rand() % bestMoves.size()];

	targetX = tileX + choice.first;
	targetY = tileY + choice.second;

	lastDirX = choice.first;
	lastDirY = choice.second;

	moving = true;

}
void Enemy::draw(SDL_Renderer* render) const {
	SDL_Rect rect;
	rect.x = static_cast<int>(pixelX);
	rect.y = static_cast<int>(pixelY);
	rect.h = TILE_SIZE;
	rect.w = TILE_SIZE;

	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	SDL_RenderFillRect(render, &rect);

}
int Enemy::getTileX()const {
	return tileX;
}
int Enemy:: getTileY()const {
	return tileY;
}

void Enemy::reset()
{
	tileX = spawnX;
	tileY = spawnY;

	pixelX = tileX * TILE_SIZE;
	pixelY = tileY * TILE_SIZE;

	targetX = tileX;
	targetY = tileY;

	moving = false;
	lastMoveTime = 0;
}
void Enemy::setPossition(int x, int y) {
	tileX = spawnX;
	tileY = spawnY;

	pixelX = tileX * TILE_SIZE;
	pixelY = tileY * TILE_SIZE;

	targetX = tileX;
	targetY = tileY;

	moving = false;
	lastMoveTime = 0;
}
void Enemy::setSpawn(int x, int y) {
	spawnX = x;
	spawnY = y;
	setPossition(x, y);
}

