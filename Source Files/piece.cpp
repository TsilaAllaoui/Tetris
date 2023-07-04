#include "piece.h"
#include "tetris.h"

Piece::Piece(const float&gameSpeed, SDL_Renderer* renderer)
{
	// Setting up the renderer
	renderer_ = renderer;

	// Starting the timer
    timer_.start();

	// Set the piece activity state
    active_ = true;

	// Setting up the piece falling speed
	speed_ = Tetris::gameSpeed * 1000;
	tmpSpeed_ = Tetris::gameSpeed * 1000;

	// Set start status
	isStarting_ = true;
}

Piece::~Piece()
{
	// Freeing memory
	tiles.clear();
	timer_.stop();

	// Unsetting attributes
	active_ = false;
	speed_ = 0;
	tmpSpeed_ = 0;
}

void Piece::update(const std::vector<Tile*>& _tiles)
{
	// Checking collision
	checkCollision(_tiles);

	// If timer exceed current speed, we move all tiles of the piece down
	if (timer_.getTime() > speed_)
	{
		if (isStarting_)
			isStarting_ = false;
		if (active_)
		{
			for (auto& tile : tiles)
				tile->move(Tile::Direction::DOWNDIR);
			timer_.start();
		}
	}
}

void Piece::show()
{
	// SHowing all tiles
	for (auto& tile : tiles)
		tile->show();
}

bool Piece::isActive()
{
	return active_;
}

bool Piece::isStarting()
{
	return isStarting_;
}

bool Piece::isCollidingUp()
{
	for (auto& tile : tiles)
		if (tile->getPosition().y <= 0)
			return true;
	return false;
}

void Piece::changeSpeed(const bool& increase)
{
	if (increase)
		speed_ = tmpSpeed_ / 25;
	else speed_ = Tetris::gameSpeed * 1000 ;
}


bool Piece::checkCollision(const std::vector<Tile*>& _tiles)
{
	auto checkTilesCollision = [](Tile* currentTile, const std::vector<Tile*>& _tiles)
	{
		for (auto& tile : _tiles)
		{
			if (tile->isCollidingWith(currentTile))
				return true;
		}
		return false;
	};

	// If one the tile of the piece is colliding
	for (auto& tile : tiles)
	{
		if ((tile->getPosition().y + 1 == Tetris::HEIGHT / Tile::Size || tile->getPosition().y <= 0 && !isStarting_) || checkTilesCollision(tile, _tiles))
		{
			active_ = false;
			return true;
		}
	}
	return false;
}

std::vector<Tile*> Piece::getTiles()
{
	return tiles;
}

bool Piece::checkLeftCollision(const std::vector<Tile*>& _tiles)
{
	for (auto& tile : tiles)
	{
		for (auto& _tile : _tiles)
		{
			auto pos = tile->getPosition();
			auto _pos = _tile->getPosition();
			if (pos.y == _pos.y && pos.x - 1 == _pos.x)
				return true;
		}
	}
	return false;
}

bool Piece::checkRightCollision(const std::vector<Tile*>& _tiles)
{
	for (auto& tile : tiles)
	{
		for (auto& _tile : _tiles)
		{
			auto pos = tile->getPosition();
			auto _pos = _tile->getPosition();
			if (pos.y == _pos.y && pos.x + 1 == _pos.x)
				return true;
		}
	}
	return false;
}

void Piece::offset(float x, float y)
{
	for (auto& tile : tiles)
	{
		auto pos = tile->getPosition();
		tile->set(pos.x + x, pos.y + y);
	}
}

int Piece::getType()
{
	return type_;
}


/*********	SQUARE PIECE	**********/

Square::Square(const float& gameSpeed, SDL_Renderer *renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::SQUARE;

	tiles.emplace_back(new Tile(7, 0, Tile::Type::SQUARE, renderer_));
	tiles.emplace_back(new Tile(7, 1, Tile::Type::SQUARE, renderer_));
	tiles.emplace_back(new Tile(8, 0, Tile::Type::SQUARE, renderer_));
	tiles.emplace_back(new Tile(8, 1, Tile::Type::SQUARE, renderer_));
}

void Square::rotatePiece()
{

}

void Square::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[2]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR && tiles[0]->getPosition().x - 1 >= 0)
	{
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}

/*********	I PIECE	**********/


IShape::IShape(const float& gameSpeed, SDL_Renderer* renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::I;

	tiles.emplace_back(new Tile(7, 0, Tile::Type::I, renderer_));
	tiles.emplace_back(new Tile(7, 1, Tile::Type::I, renderer_));
	tiles.emplace_back(new Tile(7, 2, Tile::Type::I, renderer_));
	tiles.emplace_back(new Tile(7, 3, Tile::Type::I, renderer_));
}

void IShape::rotatePiece()
{
	auto farTile = tiles[3]->getPosition();
	auto firstTile = tiles[0]->getPosition();

	if (farTile.y > firstTile.y && firstTile.x - 3 >= 0)
	{
		for (int i = 1; i < 4; i++)
		{
			tiles[i]->set(firstTile.x - i, firstTile.y);
		}
	}

	else if (farTile.y < firstTile.y && firstTile.x + 3 < Tetris::WIDTH / Tile::Size)
	{
		for (int i = 1; i < 4; i++)
		{
			tiles[i]->set(firstTile.x + i, firstTile.y);
		}
	}

	else if (farTile.x < firstTile.x && firstTile.y - 3 >= 0)
	{
		for (int i = 1; i < 4; i++)
		{
			tiles[i]->set(firstTile.x, firstTile.y - i);
		}
	}

	else if (farTile.x > firstTile.x && firstTile.y + 3 < Tetris::HEIGHT / Tile::Size)
	{
		for (int i = 1; i < 4; i++)
		{
			tiles[i]->set(firstTile.x, firstTile.y + i);
		}
	}
}


void IShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR && tiles[3]->getPosition().x - 1 >= 0)
	{
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}

/*********	L PIECE	**********/

LShape::LShape(const float& gameSpeed, SDL_Renderer* renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::L;

	tiles.emplace_back(new Tile(7, 0, Tile::Type::L, renderer_));
	tiles.emplace_back(new Tile(7, 1, Tile::Type::L, renderer_));
	tiles.emplace_back(new Tile(7, 2, Tile::Type::L, renderer_));
	tiles.emplace_back(new Tile(8, 2, Tile::Type::L, renderer_));
}

void LShape::rotatePiece()
{
	auto firstTile = tiles[0]->getPosition();
	auto pivotTile = tiles[2]->getPosition();

	if (firstTile.y < pivotTile.y && pivotTile.x + 2 < Tetris::WIDTH / Tile::Size && pivotTile.y + 1 < Tetris::HEIGHT / Tile::Size)
	{
		tiles[0]->set(pivotTile.x + 2, pivotTile.y);
		tiles[1]->set(pivotTile.x + 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y + 1);
	}

	else if (firstTile.x > pivotTile.x && pivotTile.y + 2 < Tetris::HEIGHT / Tile::Size && pivotTile.x - 1 >= 0)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y + 2);
		tiles[1]->set(pivotTile.x, pivotTile.y + 1);
		tiles[3]->set(pivotTile.x - 1, pivotTile.y);
	}

	else if (firstTile.y > pivotTile.y && pivotTile.x - 2 >= 0 && pivotTile.y - 1  >= 0)
	{
		tiles[0]->set(pivotTile.x - 2, pivotTile.y);
		tiles[1]->set(pivotTile.x - 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y - 1);
	}

	else if (firstTile.x < pivotTile.x && pivotTile.y - 2 >= 0 && pivotTile.x + 1 < Tetris::WIDTH / Tile::Size)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y - 2);
		tiles[1]->set(pivotTile.x, pivotTile.y - 1);
		tiles[3]->set(pivotTile.x + 1, pivotTile.y);
	}
}

void LShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x + 1 > Tetris::WIDTH / Tile::Size)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR)// && tiles[0]->getPosition().x - 1 >= 0)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x - 1 < 0)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}

/*********	L REVERSE PIECE	**********/

LReverseShape::LReverseShape(const float& gameSpeed, SDL_Renderer* renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::L;

	tiles.emplace_back(new Tile(8, 0, Tile::Type::LR, renderer_));
	tiles.emplace_back(new Tile(8, 1, Tile::Type::LR, renderer_));
	tiles.emplace_back(new Tile(8, 2, Tile::Type::LR, renderer_));
	tiles.emplace_back(new Tile(7, 2, Tile::Type::LR, renderer_));
}

void LReverseShape::rotatePiece()
{
	auto firstTile = tiles[0]->getPosition();
	auto pivotTile = tiles[2]->getPosition();

	if (firstTile.y < pivotTile.y && pivotTile.x + 2 < Tetris::WIDTH / Tile::Size && pivotTile.y - 1 >= 0)
	{
		tiles[0]->set(pivotTile.x + 2, pivotTile.y);
		tiles[1]->set(pivotTile.x + 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y - 1);
	}

	else if (firstTile.x > pivotTile.x && pivotTile.y + 2 < Tetris::HEIGHT / Tile::Size && pivotTile.x + 1 < Tetris::WIDTH / Tile::Size)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y + 2);
		tiles[1]->set(pivotTile.x, pivotTile.y + 1);
		tiles[3]->set(pivotTile.x + 1, pivotTile.y);
	}

	else if (firstTile.y > pivotTile.y && pivotTile.x - 2 >= 0 && pivotTile.y + 1 < Tetris::WIDTH / Tile::Size)
	{
		tiles[0]->set(pivotTile.x - 2, pivotTile.y);
		tiles[1]->set(pivotTile.x - 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y + 1);
	}

	else if (firstTile.x < pivotTile.x && pivotTile.y - 2 >= 0 && pivotTile.x - 1 >= 0)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y - 2);
		tiles[1]->set(pivotTile.x, pivotTile.y - 1);
		tiles[3]->set(pivotTile.x - 1, pivotTile.y);
	}
}

void LReverseShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x + 1 > Tetris::WIDTH / Tile::Size)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x - 1 < 0)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}


/*********	N PIECE	**********/

NShape::NShape(const float& gameSpeed, SDL_Renderer* renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::N;

	tiles.emplace_back(new Tile(7, 1, Tile::Type::N, renderer_));
	tiles.emplace_back(new Tile(7, 2, Tile::Type::N, renderer_));
	tiles.emplace_back(new Tile(8, 2, Tile::Type::N, renderer_));
	tiles.emplace_back(new Tile(8, 3, Tile::Type::N, renderer_));
}

void NShape::rotatePiece()
{
	auto firstTile = tiles[0]->getPosition();
	auto pivotTile = tiles[1]->getPosition();

	if (firstTile.y < pivotTile.y && tiles[3]->getPosition().x - 2 >= 0)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y + 1);
		tiles[3]->set(pivotTile.x - 1, pivotTile.y + 1);
	}

	else if (firstTile.y >= pivotTile.y)
	{
		tiles[0]->set(pivotTile.x , pivotTile.y - 1);
		tiles[3]->set(pivotTile.x + 1, pivotTile.y + 1);
	}
}

void NShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		for (auto& tile : tiles)
			if (tile->getPosition().x + 1 >= Tetris::WIDTH / Tile::Size)
				return;
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x - 1 < 0)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}

/*********	N REVERSE PIECE	**********/

NReverseShape::NReverseShape(const float& gameSpeed, SDL_Renderer* renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::NR;

	tiles.emplace_back(new Tile(8, 1, Tile::Type::NR, renderer_));
	tiles.emplace_back(new Tile(8, 2, Tile::Type::NR, renderer_));
	tiles.emplace_back(new Tile(7, 2, Tile::Type::NR, renderer_));
	tiles.emplace_back(new Tile(7, 3, Tile::Type::NR, renderer_));
}

void NReverseShape::rotatePiece()
{
	auto firstTile = tiles[0]->getPosition();
	auto pivotTile = tiles[1]->getPosition();

	if (firstTile.y < pivotTile.y && tiles[3]->getPosition().x + 2 < Tetris::WIDTH / Tile::Size)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y + 1);
		tiles[3]->set(pivotTile.x + 1, pivotTile.y + 1);
	}

	else if (firstTile.y >= pivotTile.y)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y - 1);
		tiles[3]->set(pivotTile.x - 1, pivotTile.y + 1);
	}
}

void NReverseShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		auto p = tiles[2]->getPosition().x;
		for (auto& tile : tiles)
			if (tile->getPosition().x + 1 >= Tetris::WIDTH / Tile::Size)
				return;
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x - 1 < 0)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}


/*********	T PIECE	**********/

TShape::TShape(const float& gameSpeed, SDL_Renderer* renderer) : Piece(gameSpeed, renderer)
{
	// Setting the type
	type_ = Tile::Type::T;

	tiles.emplace_back(new Tile(7, 2, Tile::Type::T, renderer_));
	tiles.emplace_back(new Tile(8, 2, Tile::Type::T, renderer_));
	tiles.emplace_back(new Tile(9, 2, Tile::Type::T, renderer_));
	tiles.emplace_back(new Tile(8, 1, Tile::Type::T, renderer_));
}

void TShape::rotatePiece()
{
	auto firstTile = tiles[0]->getPosition();
	auto pivotTile = tiles[1]->getPosition();

	if (firstTile.x < pivotTile.x && tiles[3]->getPosition().y < pivotTile.y && pivotTile.y + 1 < Tetris::HEIGHT / Tile::Size)
		tiles[0]->set(pivotTile.x, pivotTile.y + 1);

	else if (firstTile.y > pivotTile.y && tiles[3]->getPosition().y < pivotTile.y && pivotTile.x - 1 >= 0)
	{
		tiles[0]->set(pivotTile.x - 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y + 1);
	}

	else if (firstTile.y == pivotTile.y && tiles[3]->getPosition().y > pivotTile.y && tiles[2]->getPosition().y == pivotTile.y && pivotTile.y - 1 >= 0)
		tiles[2]->set(pivotTile.x, pivotTile.y - 1);	

	else if (tiles[3]->getPosition().y > pivotTile.y && tiles[2]->getPosition().y < pivotTile.y && pivotTile.x + 1 < Tetris::HEIGHT / Tile::Size)
	{
		tiles[2]->set(pivotTile.x + 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y - 1);
	}
}

void TShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
	{
		auto p = tiles[2]->getPosition().x;
		for (auto& tile : tiles)
			if (tile->getPosition().x + 1 >= Tetris::WIDTH / Tile::Size)
				return;
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x + 1, tile->getPosition().y);
	}
	else if (direction == Tile::Direction::LEFTDIR)
	{
		for (auto& tile : tiles)
		{
			if (tile->getPosition().x - 1 < 0)
				return;
		}
		for (auto& tile : tiles)
			tile->set(tile->getPosition().x - 1, tile->getPosition().y);
	}
}