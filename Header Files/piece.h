#pragma once

#include "timer.h"
#include "tile.h"
#include <SDL3/SDL.h>

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
        void update();
        
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
		*\ free Need to be overrided
		*\ return
		*/
		virtual bool checkCollision() = 0;
};

class Square:public Piece
{
    public:
		Square(const float& gameSpeed, SDL_Renderer *renderer);
        void rotatePiece() override;
        void move(const Tile::Direction& direction) override;
		bool checkCollision() override;
};

class IShape:public Piece
{
    public:
		IShape(const float& gameSpeed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
		bool checkCollision() override;
};

class LShape:public Piece
{
    public:
		LShape(const float& gameSpeed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
		bool checkCollision() override;
};

class LReverseShape:public Piece
{
    public:
		LReverseShape(const float& gameSpeed, SDL_Renderer* renderer);
		void rotatePiece() override;
		void move(const Tile::Direction& direction) override;
		bool checkCollision() override;
};

class N_piece:public Piece
{
    public:
        N_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
};

class N_reverse_piece:public Piece
{
    public:
        N_reverse_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
};

class T_piece:public Piece
{
    public:
        T_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
};