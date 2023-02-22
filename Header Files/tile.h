#pragma once

#include <SDL3/SDL.h>
#include <vector>

// Tile class
class Tile
{
    private:
		// Position of the tile
        SDL_FRect pos_;

		// The tile texture
		SDL_Texture* texture_;

		// The type of the tile
        int type_;

		// The renderer
		SDL_Renderer* renderer_;
    
	public:
		// The size of a square bloc composing a tile
		static const int Size;

		// Useful Enumerations
		enum Type { SQUARE, I, L, LR, N, NR, T, LEFT, RIGHT, DOWN, NONE };
		enum Direction { DOWNDIR, LEFTDIR, RIGHTDIR };


		// Default Constructor 
        Tile();

		// Destructor
		~Tile();

		/**
		*\ brief Custom Constructor
		*\ param x The x coordinate of the tile
		*\ param y The y coordinate of the tile
		*\ param type The type of the tile
		*\ param renderer The renderer responsible of drawing the tile
		*\ return
		*/
        Tile(float x, float y, Tile::Type type, SDL_Renderer* renderer);

		/**
		*\ brief Set the position of the tile
		*\ param x The x coordinate where to set the tile
		*\ param y The y coordinate where to set the tile
		*\ return
		*/
        void set(float x ,float y);

		/**
		*\ brief Move the tile in the down, left or rigth direction
		*\ param direction The direction where to move the tile
		*\ return
		*/
        void move(const Direction& direction);

		/**
		*\ brief Get the current position of the tile
		*\ return The position of the tile
		*/
        SDL_FRect getPosition();
        
		/**
		*\ brief draw the tile of the window
		*\ return
		*/
		void show();
};