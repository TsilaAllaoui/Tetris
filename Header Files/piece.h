#pragma once

#include "tile.h"
//#include <SDL/SDL_ttf.h>

// Piece class
class Piece
{
    protected:
		// The texture of the piece
		SDL_Texture* texture_;

		// The renderer
		SDL_Renderer* renderer_;

		// List of tiles composing the piece
        std::vector<Tile> tiles;

		// Piece timer
        Timer timer;

		// Piece status
        bool active, exchange, triedToSwap;

		// Piece type
        int state,type;

		// Piece speeds
        float speed, buffer_speed, tact_speed;
    
	public:
		/**
		*\ brief Constructor
		*\ param gameSpeed The current speed of the game
		*\ return
		*/
        Piece(float gameSpeed);

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
        void update(std::vector<Tile> &tiles);
        
		/**
		*\ brief Render piece on window
		*\ return
		*/
		void show();

		/**
		*\ brief R Set the position of the piece
		*\ free Need to be overrided
		*\ return
		*/
        virtual void set(float x,float y) = 0;

		/**
		*\ brief R Set the position of the piece
		*\ free Need to be overrided
		*\ return
		*/
        bool collide_left(std::vector<Tile> block_list);
        bool collide_right(std::vector<Tile> block_list);
        bool collide_down(std::vector<Tile> block_list);
        int collide_with_others(SDL_Rect mpos,std::vector<Tile> block_list);
        bool collide_when_rotating(SDL_Rect mpos,std::vector<Tile> block_list);

		/**
		*\ brief R Set the position of the piece
		*\ free Need to be overrided
		*\ return
		*/
        bool canRotate(std::vector<Tile>& tiles);
        bool is_active();
        SDL_Rect get_tile(int n);
        std::vector<Tile> get_tile_list();
        void keyhandle(std::vector<Tile> block_list);
        virtual void rotate_piece(std::vector<Tile> block_list) = 0;
        int get_type();
        bool get_exchange();
        void unset_exchange();
        void set_type();
        void lock_swap();
};

class Square_piece:public Piece
{
    public:
        Square_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
};

class I_piece:public Piece
{
    public:
        I_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
};

class L_piece:public Piece
{
    public:
        L_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
};

class L_reverse_piece:public Piece
{
    public:
        L_reverse_piece(float game_speed);
        virtual void rotate_piece(std::vector<Tile> block_list);
        virtual void set(float x,float y);
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