#pragma once

#include "timer.h"
#include "tile.h"

#include <SDL3/SDL.h>
#include <set>

// Piece class
class Piece
{
    protected:
		// The renderer
		SDL_Renderer* renderer_;

		// List of tiles composing the piece
        std::vector<Tile*> tiles;

		// Piece timer
        Timer timer_;

		// Piece status
		bool active_;

		// Piece type
        int type_;

		// Piece speeds
		float speed_, tmpSpeed_;
    
		// To know if Piece is starting 
		bool isStarting_;

	public:
		/**
		*\ brief Constructor
		*\ param gameSpeed The current speed of the game
		*\ param renderer The renderer
		*\ return
		*/
        Piece(const float& gameSpeed, SDL_Renderer* renderer);

		/**
		*\ brief Destrcutor
		*\ return
		*/
		~Piece();

		/**
		*\ brief Update the piece
		*\ param tiles The list of tiles composing the piece
		*\ return
		*/
        void update(const std::vector<Tile*>& _tiles);
        
		/**
		*\ brief Render piece on window
		*\ return
		*/
		void show();

		/**
		*\ brief Getter for active attribute
		*\ return
		*/
		bool isActive();

		/**
		*\ brief Getter for start status
		*\ return
		*/
		bool isStarting();

		/**
		*\ brief Getter for collision up status
		*\ return
		*/
		bool isCollidingUp();

		/**
		*\ brief Increase/Decrease speed of piece
		*\ param inscrease If true increase, decrease in the opposite effect
		*\ return
		*/
		void changeSpeed(const bool& increase);

		/**
		*\ brief R Set the position of the piece
		*\ param direction The direction where to move the piece
		*\ free Need to be overrided
		*\ return
		*/
        virtual void move(const Tile::Direction& direction ) = 0;

		/**
		*\ brief Rotate the piece
		*\ free Need to be overrided
		*\ return
		*/
		virtual void rotatePiece() = 0;

		/**
		*\ brief For checking collision
		*\ param tiles The game current tiles on screen
		*\ free Need to be overrided
		*\ return
		*/
		bool checkCollision(const std::vector<Tile*>& tiles);

		/**
		*\ brief For checking collision
		*\ return
		*/
		std::vector<Tile*> getTiles();
};

class Square:public Piece
{
    public:
		Square(const float& gameSpeed, SDL_Renderer *renderer);
        void rotatePiece() override;
        void move(const Tile::Direction& direction) override;
};

class IShape:public Piece
{
    public:
		IShape(const float& gameSpeed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
};

class LShape:public Piece
{
    public:
		LShape(const float& gameSpeed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
};

class LReverseShape:public Piece
{
    public:
		LReverseShape(const float& gameSpeed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
};

class NShape:public Piece
{
    public:
        NShape(const float& game_speed, SDL_Renderer* renderer);
        void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
};

class NReverseShape:public Piece
{
public:
	NReverseShape(const float& game_speed, SDL_Renderer* renderer);
	void rotatePiece() override;
	void move(const Tile::Direction& direction) override;
};

class TShape:public Piece
{
    public:
		TShape(const float& game_speed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
};