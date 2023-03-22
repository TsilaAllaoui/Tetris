#include <SDL3/SDL_image.h>
#include <set>

#include "tile.h"

const int Tile::Size = 16;

Tile::Tile()
{
    pos_.x = 0; 
	pos_.y = 0;
    pos_.w = Tile::Size;
	pos_.h = Tile::Size;
}

Tile::Tile(float x, float y,Tile::Type type, SDL_Renderer* renderer)
{
    pos_.x = x * Tile::Size;
	pos_.y = y * Tile::Size;
	pos_.w = Tile::Size;
	pos_.h = Tile::Size;
    type_ = type;
	renderer_ = renderer;
	texture_ = IMG_LoadTexture(renderer_, "./data/images/tile.png");
}

void Tile::move(const Tile::Direction &direction)
{
	// If moving down
	if (direction == Tile::Direction::DOWNDIR)
		pos_.y += Tile::Size;
	
	// If moving left
	else if (direction == Tile::Direction::LEFTDIR)
		pos_.x -= Tile::Size;
	
	// If moving right
	else if (direction == Tile::Direction::RIGHTDIR)
		pos_.x += Tile::Size;
}

void Tile::show()
{
	// The position of blitting of the tile
    SDL_Rect mpos;
    mpos.y = 0; 
	mpos.w = Tile::Size; 
	mpos.h = Tile::Size;

	// Updating the blitting position by the type of the tile
    if (type_ == Type::SQUARE)
        mpos.x = 0;
    else if (type_ == Type::I)
        mpos.x = Tile::Size;
    else if (type_ == L)
        mpos.x = 2*Tile::Size;
    else if (type_ == LR)
        mpos.x = 3*Tile::Size;
    else if (type_ == N)
        mpos.x = 4*Tile::Size;
    else if (type_ == NR)
        mpos.x = 5*Tile::Size;
    else if (type_ == T)
        mpos.x = 6*Tile::Size;

	// Render the tile
	SDL_RenderTexture(renderer_, texture_, &mpos, &pos_);
}

bool Tile::isCollidingWith(Tile *tile)
{
	auto x = tile->getPosition().x;
	auto y = tile->getPosition().y;
	auto x_ = this->getPosition().x;
	auto y_ = this->getPosition().y;
	if (x == x_ && y + 1 == y_)
		return true;
	return false;
}

SDL_FRect Tile::getPosition()
{
	return { pos_.x / 16, pos_.y / 16, pos_.w, pos_.h };
}

void Tile::set(float x ,float y)
{
    pos_.x = x * Tile::Size;
	pos_.y = y * Tile::Size;
}

Tile::~Tile()
{
	// Freeing memory
	SDL_DestroyTexture(texture_);
}