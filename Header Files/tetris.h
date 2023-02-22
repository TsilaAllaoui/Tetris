#pragma once

#include <SDL3/SDL.h>
#include "timer.h"
#include "tile.h"
//#include "piece.h"

// Tetris game class
class Tetris
{
    private:
		// The window and the renderer
		SDL_Window* window_;
		SDL_Renderer* renderer_;

		// Textures
        SDL_Texture *sprite, *background, *score, *titlescreen;

		// Main timer for the game
        Timer timer;

		// The current score
        int CurrentScore;

		//  The current game speed
        float gameSpeed;

		// The position of sprites
        SDL_FRect pos_b;

		/*
        Piece *curr_piece, *next_piece, *stock_piece, *UI_piece, *UI_stock_piece;
        vector<Tile> block_list, tmp;
        TTF_Font *police;
		*/

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
		*\ brief To check if the current game is over
		*\ return true if the current game encountered a gameover
		*/
        bool isGameOver();

		/**
		*\ brief Erase all possible lines
		*\ return
		*/
        void erasePossibleLines();

		/**
		*\ brief Erase an element at a given position
		*\ param mpos The position of the element to be erased
		*\ return
		*/
        void eraseElement(SDL_Rect mpos);

		/**
		*\ brief Show current Piece in the UI
		*\ return
		*/
        void showUiPiece();

		/**
		*\ brief Show stocked Piece
		*\ return
		*/
        void showStockedPiece();

		/**
		*\ brief Show game UI
		*\ return
		*/
        void showUI();

		/**
		*\ brief Init Tetris game
		*\ return
		*/
        void init();

		/**
		*\ brief End Tetris game
		*\ return
		*/
        bool stop();
        /*
        int get_score_long();
		bool is_at(float mx,float my);
		Piece* gen_next_Piece();
		*/
};