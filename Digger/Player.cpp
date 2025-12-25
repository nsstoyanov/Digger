#include "Player.h"
Player::Player() {

	tileX = 0;
	tileY = 0;

	pixelX = 0.0f;
	pixelY = 0.0f;

	targetX = 0;
	targetY = 0;

	moving = false;
	lastMoveTime = 0;

	lives = 3;

	spawnX = 0;
	spawnY = 0;
}
void Player::handleInput(const Uint8* keys, Map& map) {

	if (moving)
		return;

	Uint32 now = SDL_GetTicks();
	if (now - lastMoveTime < MOVE_DELAY) {
		return;
	}

	int dx = 0, dy = 0;

	if (keys[SDL_SCANCODE_UP])        dy = -1;
	else if (keys[SDL_SCANCODE_DOWN]) dy = 1;
	else if (keys[SDL_SCANCODE_LEFT]) dx = -1;
	else if (keys[SDL_SCANCODE_RIGHT]) dx = 1;

	if (dx == 0 && dy == 0)
		return;

	int nx = tileX + dx;
	int ny = tileY + dy;
	if (map.isWalkable(nx, ny))
	{
		targetX = nx;
		targetY = ny;
		moving = true;
		lastMoveTime = now;

		map.dig(nx, ny);
	}
}
void Player::update()
{
	if (!moving)
		return;

	float targetPixelX = targetX * TILE_SIZE;
	float targetPixelY = targetY * TILE_SIZE;

	float dx = targetPixelX - pixelX;
	float dy = targetPixelY - pixelY;

	float distance = SDL_sqrt(dx * dx + dy * dy);

	float step = MOVE_SPEED * (1.0f / 60.0f); 

	if (distance <= step)
	{
		pixelX = targetPixelX;
		pixelY = targetPixelY;

		tileX = targetX;
		tileY = targetY;

		moving = false;
	}
	else
	{
		pixelX += dx / distance * step;
		pixelY += dy / distance * step;
	}
}

void Player::draw(SDL_Renderer* render) const {
	SDL_Rect rect;
	rect.x = static_cast<int>(pixelX);
	rect.y = static_cast<int>(pixelY);
	rect.w = TILE_SIZE;
	rect.h = TILE_SIZE;

	SDL_SetRenderDrawColor(render, 255, 255, 0, 255);
	SDL_RenderFillRect(render, &rect);

}
int Player::getTileX() const
{
	return tileX;
}

int Player::getTileY() const
{
	return tileY;
}

void Player::loseLife()
{
	lives--;
	if (lives > 0)
		respawn();
}

bool Player::isAlive() const
{
	return lives > 0;
}

void Player::respawn()
{
	setPossition(spawnX, spawnY);
}

int Player::getLives() const
{
	return lives;
}

void Player::setPossition(int x, int y)
{
	tileX = x;
	tileY = y;

	pixelX = tileX * TILE_SIZE;
	pixelY = tileY * TILE_SIZE;

	targetX = tileX;
	targetY = tileY;

	moving = false;
}
void Player::setSpawn(int x, int y)
{
	spawnX = x;
	spawnY = y;
	setPossition(x, y); 
}