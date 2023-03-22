
#include<SDL3/SDL.h>
#include <ctime>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL.h>

#include "tetris.h"
//#include <SDL/SDL_ttf.h>

const int Tetris::WIDTH = 272;
const int Tetris::HEIGHT = 352;
const int Tetris::S_WIDTH = 160;
const int Tetris::FPS_CAP = 60;
int Tetris::gameSpeed = 1;

Tetris::Tetris()
{
	// For randomness
    std::srand(std::time(0));

	// Creating window
	window_ = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Tetris::WIDTH, Tetris::HEIGHT, SDL_WINDOW_INPUT_FOCUS);

	// Creating renderer
	renderer_ = SDL_CreateRenderer(window_, NULL, 0);

	// Loading assets
	sprite = IMG_LoadTexture(renderer_, "./data/images/tile.png");
    background = IMG_LoadTexture(renderer_, "./data/images/bg.png");
    titlescreen = IMG_LoadTexture(renderer_, "./data/images/titlescreen.png");

	// Setting sprite position
    pos_b.x = Tetris::S_WIDTH + Tile::Size;
    pos_b.y = 0;

	// Setting current score
    CurrentScore = 0;

	// Intanciating attributes
	activePiece = nullptr;
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
    while(/*!isGameOver() &&*/ !isGameOver_)
    {
		// update timer
        timer.start();

		// Update the game objects
        update();

		// Render game objects
        show();

		// For constant FPS
        if ((1000/Tetris::FPS_CAP) > timer.getTime())
            SDL_Delay((1000/Tetris::FPS_CAP) - timer.getTime());
    }
}

void Tetris::init()
{
	// Creating the first piece and make it as the active piece
	Piece *piece = generateRandomPiece();
	pieces_.emplace_back(piece);
	activePiece = pieces_.at(pieces_.size() - 1);
}

void Tetris::update()
{
	// Get key events
	handleKey();

	// Moving active piece down
	activePiece->update(tiles_);

	// Check if current piece is colliding underneath
	if (!activePiece->isActive())
	{
		if (activePiece->isCollidingUp())
		{
			isGameOver_ = true;
			return;
		}

		auto currentTiles = activePiece->getTiles();
		for (auto& tile : currentTiles)
			tiles_.emplace_back(tile);
		Piece* piece = generateRandomPiece();
		pieces_.emplace_back(piece);
		activePiece = pieces_.at(pieces_.size() - 1);
	}
}

void Tetris::show()
{
	// Clearing window
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	SDL_RenderClear(renderer_);

	// Showing pieces
	for (auto& piece : pieces_)
		piece->show();

	// Updating window
	SDL_RenderPresent(renderer_);
}

bool Tetris::isGameOver()
{
	if (activePiece->isCollidingUp())
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
			if (event.key.keysym.sym == SDLK_LEFT)
				activePiece->move(Tile::Direction::LEFTDIR);
			
			// If moving right
			if (event.key.keysym.sym == SDLK_RIGHT)
				activePiece->move(Tile::Direction::RIGHTDIR);

			// If moving down faster
			if (event.key.keysym.sym == SDLK_DOWN)
				activePiece->changeSpeed(true);

			// If rotating
			if (event.key.keysym.sym == SDLK_SPACE && activePiece->isActive())
				activePiece->rotatePiece();

		}

		else if (event.type == SDL_EVENT_KEY_UP)
		{
			// If moving down faster
			if (event.key.keysym.sym == SDLK_DOWN)
				activePiece->changeSpeed(false);
		}
	}
}

Piece* Tetris::generateRandomPiece()
{
	int value = rand() % 7;
	Tile::Type type;
	Piece *piece = nullptr;
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

	return piece;
}
