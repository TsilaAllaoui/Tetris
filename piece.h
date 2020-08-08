#ifndef PIECE_H
#define PIECE_H

#include "defs.h"
#include "tile.h"
#include <SDL/SDL_ttf.h>


class Piece
{
    protected:
        vector<Tile> tile_list;
        Timer timer;
        bool active,exchange,tried_to_swap;
        int state,type;
        float speed, buffer_speed, tact_speed;
    public:
        Piece(float game_speed);
        void update(vector<Tile> &block_list);
        void show(SDL_Surface *screen,SDL_Surface *sprite);
        virtual void set(float x,float y) = 0;
        bool collide_left(vector<Tile> block_list);
        bool collide_right(vector<Tile> block_list);
        bool collide_down(vector<Tile> block_list);
        int collide_with_others(SDL_Rect mpos,vector<Tile> block_list);
        bool collide_when_rotating(SDL_Rect mpos,vector<Tile> block_list);
        bool test_rotation(vector<Tile> block_list);
        bool is_active();
        SDL_Rect get_tile(int n);
        vector<Tile> get_tile_list();
        void keyhandle(vector<Tile> block_list);
        virtual void rotate_piece(vector<Tile> block_list) = 0;
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
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

class I_piece:public Piece
{
    public:
        I_piece(float game_speed);
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

class L_piece:public Piece
{
    public:
        L_piece(float game_speed);
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

class L_reverse_piece:public Piece
{
    public:
        L_reverse_piece(float game_speed);
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

class N_piece:public Piece
{
    public:
        N_piece(float game_speed);
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

class N_reverse_piece:public Piece
{
    public:
        N_reverse_piece(float game_speed);
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

class T_piece:public Piece
{
    public:
        T_piece(float game_speed);
        virtual void rotate_piece(vector<Tile> block_list);
        virtual void set(float x,float y);
};

#endif // PIECE_H
