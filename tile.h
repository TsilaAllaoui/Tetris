#ifndef TILE_H
#define TILE_H

#include "defs.h"
#include "timer.h"

class Tile
{
    private:
        SDL_Rect pos;
        int color;
    public:
        Tile();
        Tile(float x, float y,int mcolor);
        void set(float x ,float y);
        void move_down();
        void move_left();
        void move_right();
        SDL_Rect get_pos();
        void show(SDL_Surface *screen,SDL_Surface *sprite);
        ~Tile();
        bool collide_with_others(vector<Tile> block_list);
};

#endif // TILE_H
