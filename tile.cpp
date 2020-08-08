#include "tile.h"

Tile::Tile()
{
    pos.x = 0; pos.y = 0;
    pos.w = BLOC; pos.h = BLOC;
}

Tile::Tile(float x, float y,int mcolor)
{
    pos.x = x; pos.y = y;
    color = mcolor;
}

void Tile::move_down()
{
    pos.y += BLOC;
}

void Tile::move_left()
{
    pos.x -= BLOC;
}

void Tile::move_right()
{
    pos.x += BLOC;
}

void Tile::show(SDL_Surface *screen,SDL_Surface *sprite)
{
    SDL_Rect mpos;
    mpos.y = 0; mpos.w = BLOC; mpos.h = BLOC;
    if (color == SQUARE)
        mpos.x = 0;
    else if (color == I)
        mpos.x = BLOC;
    else if (color == L)
        mpos.x = 2*BLOC;
    else if (color == LR)
        mpos.x = 3*BLOC;
    else if (color == N)
        mpos.x = 4*BLOC;
    else if (color == NR)
        mpos.x = 5*BLOC;
    else if (color == T)
        mpos.x = 6*BLOC;
    SDL_BlitSurface(sprite,&mpos,screen,&pos);
}

SDL_Rect Tile::get_pos()
{
    return pos;
}

void Tile::set(float x ,float y)
{
    pos.x = x; pos.y = y;
}

Tile::~Tile()
{
    pos.x = -500; pos.y = -500;
}

bool Tile::collide_with_others(vector<Tile> block_list)
{
    for (int i=0;i<(int)block_list.size();i++)
    {
        SDL_Rect bpos = block_list[i].get_pos();
        if ((pos.y + BLOC == bpos.y && pos.x == bpos.x) || (pos.y + BLOC == HEIGHT))
            return true;
    }
    return false;
}
