#pragma once

#include <SDL3/SDL.h>
#include <stack>
#include <set>

#include "timer.h"
#include "piece.h"
#include "tile.h"

// Tetris game class
class Tetris
{
    private:
		// The window and the renderer
		SDL_Window* window_;
		SDL_Renderer* renderer_;

		// Textures
        SDL_Texture *sprite, *background, *score, *titlescreen, *ui;

		// Main timer for the game
        Timer timer_;

		// The current score
        int CurrentScore;

		// The position of sprites
        SDL_FRect pos_b;

		// All pieces of the tetris game
		std::vector<Piece*> pieces_;

		// The tiles currently in the game
		std::vector<Tile*> tiles_;

		// The active Piece
		Piece* activePiece_;

		// The Stored Piece
		Piece* storedPiece_;

		// The Next Piece
		Piece* nextPiece_;

		// Chech if game over
		bool isGameOver_;
    public:

		/**
		*\ brief Default Constructor
		*\ return
		*/
        Tetris();

		/**
		*\ brief Destructor
		*\ return
		*/
		~Tetris();

		// Useful constants
		static const int WIDTH, HEIGHT, S_WIDTH, FPS_CAP;

		static int gameSpeed;

		/**
		*\ brief Begin the Tetris game
		*\ return
		*/
        void play();

		/**
		*\ brief Update game objects 
		*\ return
		*/
        void update();

		/**
		*\ brief Render game objects
		*\ return
		*/
        void show();

		/**
		*\ brief Render game ui
		*\ return
		*/
		void showUi();

		/**
		*\ brief To check if the current game is over
		*\ return true if the current game encountered a gameover
		*/
        bool isGameOver();

		/**
		*\ brief Init Tetris game
		*\ return
		*/
        void init();

		/**
		*\ brief For handling key events
		*\ return
		*/
		void handleKey();

		/**
		*\ brief Generate a random piece
		*\ return
		*/
		Piece* generateRandomPiece();

		/**
		*\ brief Generate specified piece
		*\ The type of the piece
		*\ return The newly created specified instance of a piece
		*/
		Piece* generatePiece(int type);

		/**
		*\ brief Erase possible lines
		*\ return
		*/
		void eraseLines();

		/**
		*\ brief reposition next piece on the UI
		*\ return
		*/
		void repositionNextPiece();

		/**
		*\ brief reposition stored piece on the UI
		*\ return
		*/
		void repositionStoredPiece();

		/**
		*\ brief Store piece
		*\ return
		*/
		void storePiece();
};