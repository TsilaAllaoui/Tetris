
#include <SDL3/SDL_image.h>
#include <SDL3/SDL.h>
#include<SDL3/SDL.h>
#include <ctime>
#include <map>

#include "tetris.h"
//#include <SDL/SDL_ttf.h>

const int Tetris::WIDTH = 176;
const int Tetris::HEIGHT = 320;
const int Tetris::S_WIDTH = 240;
const int Tetris::FPS_CAP = 60;
int Tetris::gameSpeed = 1;

Tetris::Tetris()
{
	// For randomness
	std::srand(std::time(0));

	// Creating window
	window_ = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Tetris::S_WIDTH, Tetris::HEIGHT, SDL_WINDOW_INPUT_FOCUS);

	// Creating renderer
	renderer_ = SDL_CreateRenderer(window_, NULL, 0);

	// Loading assets
	sprite = IMG_LoadTexture(renderer_, "./data/images/tile.png");
	background = IMG_LoadTexture(renderer_, "./data/images/bg.png");
	titlescreen = IMG_LoadTexture(renderer_, "./data/images/titlescreen.png");
	ui = IMG_LoadTexture(renderer_, "./data/images/ui.png");

	// Setting sprite position
	pos_b.x = Tetris::S_WIDTH + Tile::Size;
	pos_b.y = 0;

	// Setting current score
	CurrentScore = 0;

	// Intanciating attributes
	activePiece_ = nullptr;
	storedPiece_ = nullptr;
	nextPiece_ = nullptr;
	score = 0;

	// Setting up game ove state
	isGameOver_ = false;
}

Tetris::~Tetris()
{
	//Freeing memory
	SDL_DestroyTexture(sprite);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(titlescreen);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
}

void Tetris::play()
{
	// Init the game
	init();

	// Main game loop
	while (/*!isGameOver() &&*/ !isGameOver_)
	{
		// update timer
		timer_.start();

		// Update the game objects
		// Update the game objects
		update();

		// Render game objects
		show();

		// For constant FPS
		if ((1000 / Tetris::FPS_CAP) > timer_.getTime())
			SDL_Delay((1000 / Tetris::FPS_CAP) - timer_.getTime());
	}
}

void Tetris::init()
{
	// Creating the first piece and make it as the active piece
	Piece* piece = generateRandomPiece();
	pieces_.emplace_back(piece);
	activePiece_ = pieces_.at(pieces_.size() - 1);
	activePiece_->offset(-2);

	// Creating stored piece
	repositionStoredPiece();
}

void Tetris::repositionStoredPiece()
{
	nextPiece_ = generateRandomPiece();
	float x = 0, y = 0;
	if (nextPiece_->getType() == Tile::Type::SQUARE) { x = 5.15; y = 2.5; }
	else if (nextPiece_->getType() == Tile::Type::I) { x = 5.5; y = 1.6; }
	else if (nextPiece_->getType() == Tile::Type::T) { x = 4.5; y = 1.5; }
	else if (nextPiece_->getType() == Tile::Type::N || nextPiece_->getType() == Tile::Type::NR) { x = 5; y = 1; }
	else if (nextPiece_->getType() == Tile::Type::L || nextPiece_->getType() == Tile::Type::LR) { x = 5; y = 2; }
	nextPiece_->offset(x, y);
}

void Tetris::update()
{
	// Get key events
	handleKey();

	// Moving active piece down
	activePiece_->update(tiles_);

	// Check if current piece is colliding underneath
	if (!activePiece_->isActive())
	{
		if (activePiece_->isCollidingUp())
		{
			isGameOver_ = true;
			return;
		}

		auto currentTiles = activePiece_->getTiles();
		for (auto& tile : currentTiles)
			tiles_.emplace_back(tile);
		activePiece_ = generatePiece(nextPiece_->getType());
		repositionStoredPiece();
		pieces_.emplace_back(nextPiece_);

		// Check if lines need to be erased
		eraseLines();
	}
}

void Tetris::show()
{
	// Clearing window
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_RenderClear(renderer_);

	activePiece_->show();

	// Showing tiles
	for (auto& tile : tiles_)
		tile->show();

	// Showing ui
	showUi();

	// Show next piece
	nextPiece_->show();

	// Show stored piece if there is any
	if (storedPiece_)
		storedPiece_->show();

	// Updating window
	SDL_RenderPresent(renderer_);
}

void Tetris::showUi()
{
	SDL_FRect pos;
	pos.x = Tetris::WIDTH;
	pos.y = 0;
	pos.h = Tetris::HEIGHT;
	pos.w = 66;
	SDL_RenderTexture(renderer_, ui, NULL, &pos);
}

bool Tetris::isGameOver()
{
	if (activePiece_->isCollidingUp())
		return true;
	return false;
}

void Tetris::handleKey()
{
	// Listening for events
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_KEY_DOWN)
		{
			// If the user press ESC key, quit the game
			if (event.key.keysym.sym == SDLK_ESCAPE)
				exit(EXIT_SUCCESS);

			// If moving left
			if (event.key.keysym.sym == SDLK_LEFT && !activePiece_->checkLeftCollision(tiles_))
				activePiece_->move(Tile::Direction::LEFTDIR);

			// If moving right
			if (event.key.keysym.sym == SDLK_RIGHT && !activePiece_->checkRightCollision(tiles_))
				activePiece_->move(Tile::Direction::RIGHTDIR);

			// If moving down faster
			if (event.key.keysym.sym == SDLK_DOWN)
				activePiece_->changeSpeed(true);

			// If rotating
			if (event.key.keysym.sym == SDLK_SPACE && activePiece_->isActive())
				activePiece_->rotatePiece();

		}

		else if (event.type == SDL_EVENT_KEY_UP)
		{
			// If moving down faster
			if (event.key.keysym.sym == SDLK_DOWN)
				activePiece_->changeSpeed(false);
		}
	}
}

Piece* Tetris::generateRandomPiece()
{
	// Generate random number
	int value = rand() % 7;
	Tile::Type type;

	// Creating random piece
	Piece* piece = nullptr;
	switch (value)
	{
	case 0: type = Tile::Type::SQUARE; piece = new Square(gameSpeed, renderer_); break;
	case 1: type = Tile::Type::I; piece = new IShape(gameSpeed, renderer_); break;
	case 2: type = Tile::Type::L; piece = new LShape(gameSpeed, renderer_); break;
	case 3: type = Tile::Type::LR; piece = new LReverseShape(gameSpeed, renderer_); break;
	case 4: type = Tile::Type::N; piece = new NShape(gameSpeed, renderer_); break;
	case 5: type = Tile::Type::NR; piece = new NReverseShape(gameSpeed, renderer_); break;
	case 6: type = Tile::Type::T; piece = new TShape(gameSpeed, renderer_); break;
	}

	//return new Square(gameSpeed, renderer_);
	return piece;
}

Piece* Tetris::generatePiece(int type)
{
	// Creating random piece
	Piece* piece = nullptr;
	switch (type)
	{
	case 0: type = Tile::Type::SQUARE; piece = new Square(gameSpeed, renderer_); break;
	case 1: type = Tile::Type::I; piece = new IShape(gameSpeed, renderer_); break;
	case 2: type = Tile::Type::L; piece = new LShape(gameSpeed, renderer_); break;
	case 3: type = Tile::Type::LR; piece = new LReverseShape(gameSpeed, renderer_); break;
	case 4: type = Tile::Type::N; piece = new NShape(gameSpeed, renderer_); break;
	case 5: type = Tile::Type::NR; piece = new NReverseShape(gameSpeed, renderer_); break;
	case 6: type = Tile::Type::T; piece = new TShape(gameSpeed, renderer_); break;
	}

	//return new Square(gameSpeed, renderer_);
	piece->offset(-2);
	return piece;
}

void Tetris::eraseLines()
{
	// Get all leftmost tiles
	std::vector<Tile*> leftMostTiles;
	for (auto& tile : tiles_)
		if (tile->getPosition().x == 0)
			leftMostTiles.emplace_back(tile);

	// The list of pair of (leftmost tile/list of tiles in the same line)
	std::map<Tile*, std::vector<Tile*>> toErase;

	// Feeding the list
	for (auto& leftTile : leftMostTiles)
	{
		std::vector<Tile*> tmp;
		for (auto& tile : tiles_)
		{
			if (tile == leftTile)
				continue;

			if (tile->getPosition().y == leftTile->getPosition().y)
				tmp.emplace_back(tile);
		}
		if (tmp.size() == Tetris::WIDTH / Tile::Size - 1)
			toErase[leftTile].insert(toErase[leftTile].end(), tmp.begin(), tmp.end());
	}

	// If there is at least one line to be erased
	if (toErase.size() > 0)
	{
		// Comparing by y position the leftmost tile
		struct comp
		{
			bool operator()(Tile* t1, Tile* t2) const
			{
				return t1->getPosition().y > t2->getPosition().y;
			}
		};

		// List of all pair sorted by y position of the leftmost tile
		std::map<Tile*, std::vector<Tile*>, comp> sortedTiles;
		for (auto& toErasePair : toErase)
			sortedTiles[toErasePair.first] = toErasePair.second;

		// Moving all remaining tiles down
		for (auto& pair : sortedTiles)
		{
			tiles_.erase(std::find(tiles_.begin(), tiles_.end(), pair.first));
			for (auto& tile : pair.second)
				tiles_.erase(std::find(tiles_.begin(), tiles_.end(), tile));
			for (auto& tile : tiles_)
				if (tile->getPosition().y < pair.first->getPosition().y)
					tile->move(Tile::Direction::DOWNDIR);
		}

		// Little delay
		SDL_Delay(800);
	}
}
