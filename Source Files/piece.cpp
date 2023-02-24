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

void Piece::update()
{
	// Checking collision
	checkCollision();

	// If timer exceed current speed, we move all tiles of the piece down
	if (active_ && timer_.getTime() > speed_)
	{
		for (auto& tile : tiles)
			tile->move(Tile::Direction::DOWNDIR);
		timer_.start();
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

void Piece::changeSpeed(const bool& increase)
{
	if (increase)
		speed_ = tmpSpeed_ / 25;
	else speed_ = Tetris::gameSpeed * 1000 ;
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

bool Square::checkCollision()
{
	// If the piece is colliding on the downside
	if (tiles[1]->getPosition().y + 1 == Tetris::HEIGHT / Tile::Size || tiles[3]->getPosition().y + 1 == Tetris::HEIGHT / Tile::Size)
	{
		active_ = false;
		return true;
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

bool IShape::checkCollision()
{
	// If the piece is colliding on the downside
	if (tiles[3]->getPosition().y + 1 >= Tetris::HEIGHT / Tile::Size || tiles[0]->getPosition().y + 1 >= Tetris::HEIGHT / Tile::Size)
	{
		active_ = false;
		return true;
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
	auto farTile = tiles[3]->getPosition();
	auto firstTile = tiles[0]->getPosition();
	auto pivotTile = tiles[2]->getPosition();

	if (firstTile.y < pivotTile.y && pivotTile.x + 3 < Tetris::WIDTH / Tile::Size)
	{
		tiles[0]->set(pivotTile.x + 2, pivotTile.y);
		tiles[1]->set(pivotTile.x + 1, pivotTile.y);
		tiles[3]->set(pivotTile.x, pivotTile.y + 1);
	}

	if (firstTile.x > pivotTile.x && pivotTile.y + 3 < Tetris::WIDTH / Tile::Size)
	{
		tiles[0]->set(pivotTile.x, pivotTile.y + 2);
		tiles[1]->set(pivotTile.x, pivotTile.y + 1);
		tiles[3]->set(pivotTile.x - 1, pivotTile.y);
	}

	/*else if (farTile.y < firstTile.y && firstTile.x + 3 < Tetris::WIDTH / Tile::Size)
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
	}*/
}

void LShape::move(const Tile::Direction& direction)
{
	if (!active_)
		return;

	if (direction == Tile::Direction::RIGHTDIR && tiles[3]->getPosition().x + 1 < Tetris::WIDTH / Tile::Size)
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

bool LShape::checkCollision()
{
	// If the piece is colliding on the downside
	if (tiles[1]->getPosition().y + 1 == Tetris::HEIGHT / Tile::Size || tiles[3]->getPosition().y + 1 == Tetris::HEIGHT / Tile::Size)
	{
		active_ = false;
		return true;
	}
}