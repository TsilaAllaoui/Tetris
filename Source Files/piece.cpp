#include "piece.h"

Piece::Piece(float game_speed)
{
    tried_to_swap = false;
    state = 0;
    timer.start();
    active = true;
    tact_speed = 10;
    speed = game_speed;
    buffer_speed = game_speed;
    exchange = false;
}

void Piece::update(vector<Tile> &block_list)
{
    keyhandle(block_list);
    if (timer.get_time() > speed)
    {
        if (!collide_down(block_list))
        for (int i=0;i<(int)tile_list.size();i++)
                tile_list[i].move_down();
        timer.start();
    }
}

void Piece::show(SDL_Surface *screen, SDL_Surface *sprite)
{

       for (int i=0;i<tile_list.size();i++)
           tile_list[i].show(screen,sprite);
}

bool Piece::collide_left(vector<Tile> block_list)
{
        if (tile_list[0].get_pos().x <= BLOC || collide_with_others(tile_list[0].get_pos(),block_list) == LEFT
            || tile_list[1].get_pos().x <= BLOC || collide_with_others(tile_list[1].get_pos(),block_list) == LEFT
            || tile_list[2].get_pos().x <= BLOC || collide_with_others(tile_list[2].get_pos(),block_list) == LEFT
            ||tile_list[3].get_pos().x <= BLOC || collide_with_others(tile_list[3].get_pos(),block_list) == LEFT)
            return true;
    return false;
}

bool Piece::collide_right(vector<Tile> block_list)
{
        if (tile_list[0].get_pos().x >= S_WIDHT || collide_with_others(tile_list[0].get_pos(),block_list) == RIGHT
            || tile_list[1].get_pos().x >= S_WIDHT || collide_with_others(tile_list[1].get_pos(),block_list) == RIGHT
            || tile_list[2].get_pos().x >= S_WIDHT || collide_with_others(tile_list[2].get_pos(),block_list) == RIGHT
            ||tile_list[3].get_pos().x >= S_WIDHT || collide_with_others(tile_list[3].get_pos(),block_list) == RIGHT)
            return true;
    return false;
}

bool Piece::collide_down(vector<Tile> block_list)
{
    if (tile_list[0].get_pos().y == HEIGHT-2*BLOC || collide_with_others(tile_list[0].get_pos(),block_list) == DOWN
        || tile_list[1].get_pos().y == HEIGHT-2*BLOC || collide_with_others(tile_list[1].get_pos(),block_list) == DOWN
        || tile_list[2].get_pos().y == HEIGHT-2*BLOC || collide_with_others(tile_list[2].get_pos(),block_list) == DOWN
        || tile_list[3].get_pos().y == HEIGHT-2*BLOC || collide_with_others(tile_list[3].get_pos(),block_list) == DOWN)
    {
        active = false;
        return true;
    }
    return false;
}

int Piece::collide_with_others(SDL_Rect mpos,vector<Tile> block_list)
{
    for (int i=0;i<(int)block_list.size();i++)
    {
        if (mpos.y + BLOC >= block_list[i].get_pos().y && mpos.y + BLOC <= block_list[i].get_pos().y + BLOC && mpos.x == block_list[i].get_pos().x)
            return DOWN;
        if (mpos.x + BLOC >= block_list[i].get_pos().x && mpos.x + BLOC <= block_list[i].get_pos().x + BLOC && mpos.y == block_list[i].get_pos().y)
            return RIGHT;
        if (mpos.x <= block_list[i].get_pos().x + BLOC && mpos.x >= block_list[i].get_pos().x && mpos.y == block_list[i].get_pos().y)
            return LEFT;
    }
    return NONE;
}

bool Piece::is_active()
{
    return active;
}

int Piece::get_type()
{
    return type;
}

SDL_Rect Piece::get_tile(int n)
{
    return tile_list[n].get_pos();
}

vector<Tile> Piece::get_tile_list()
{
    return tile_list;
}

void Piece::keyhandle(vector<Tile> block_list)
{
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym ==  SDLK_ESCAPE)
                exit(EXIT_SUCCESS);
            if (event.key.keysym.sym == SDLK_LEFT && active)
                if (!collide_left(block_list))
                    for (int i=0;i<(int)tile_list.size();i++)
                       tile_list[i].move_left();
            if (event.key.keysym.sym == SDLK_RIGHT && active)
                if (!collide_right(block_list))
                    for (int i=0;i<(int)tile_list.size();i++)
                       tile_list[i].move_right();
            if (event.key.keysym.sym == SDLK_SPACE)
                rotate_piece(block_list);
            if (event.key.keysym.sym == SDLK_DOWN)
                speed = tact_speed;
            if (event.key.keysym.sym == SDLK_s && !tried_to_swap)
            {
                exchange = true;
                tried_to_swap = true;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.sym == SDLK_DOWN)
                    speed = buffer_speed;
        }
    }
}

bool Piece::collide_when_rotating(SDL_Rect mpos,vector<Tile> block_list)
{
    for (int i=0;i<(int)block_list.size();i++)
    {
        SDL_Rect bpos = block_list[i].get_pos();
        if (mpos.y == bpos.y && mpos.x == bpos.x)
            return true;
    }
    return false;
}

bool Piece::test_rotation(vector<Tile> block_list)
{
   for (int i=0;i<(int)tile_list.size();i++)
    {
        if (tile_list[i].get_pos().x + BLOC > S_WIDHT || tile_list[i].get_pos().x < 0
            || tile_list[i].get_pos().y + BLOC > HEIGHT - BLOC || collide_when_rotating(tile_list[i].get_pos(),block_list)
            || tile_list[i].get_pos().y < BLOC)
            return true;
    }
    return false;
}

bool Piece::get_exchange()
{
    return exchange;
}

void Piece::unset_exchange()
{
    exchange = false;
}

void Piece::lock_swap()
{
    tried_to_swap = true;
}

Square_piece::Square_piece(float game_speed):Piece(game_speed)
{
    type = SQUARE;
    for (int i=0;i<=1;i++)
    {
        Tile tile((5+i)*BLOC,BLOC,SQUARE);
        tile_list.push_back(tile);
    }
    for (int i=0;i<=1;i++)
    {
        Tile tile((5+i)*BLOC,2*BLOC,SQUARE);
        tile_list.push_back(tile);
    }
}

void Square_piece::rotate_piece(vector<Tile> block_list)
{

}

void Square_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x+BLOC,y);
    tile_list[2].set(x,y+BLOC);
    tile_list[3].set(x+BLOC,y+BLOC);
}

I_piece::I_piece(float game_speed):Piece(game_speed)
{
    state = 1;
    type = I;
    for (int i=1;i<5;i++)
    {
        Tile tile(5*BLOC,i*BLOC,I);
        tile_list.push_back(tile);
    }
}

void I_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x,y+BLOC);
    tile_list[2].set(x,y+2*BLOC);
    tile_list[3].set(x,y+3*BLOC);
}

void I_piece::rotate_piece(vector<Tile> block_list)
{
    vector<Tile> tmp;
    tmp = tile_list;
    if (state == 0)
    {
        tile_list[0].set(tile_list[2].get_pos().x,tile_list[2].get_pos().y - 2*BLOC);
        for (int i=1;i<=3;i++)
            tile_list[i].set(tile_list[0].get_pos().x,tile_list[0].get_pos().y + i*BLOC);
        if (!test_rotation(block_list))
        {
            state = 1;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 1)
    {
        tile_list[0].set(tile_list[0].get_pos().x - 2*BLOC,tile_list[0].get_pos().y + 2*BLOC);
        for (int i=1;i<=3;i++)
            tile_list[i].set(tile_list[0].get_pos().x + i*BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 0;
            return;
        }
        tile_list = tmp;
    }
}

L_piece::L_piece(float game_speed):Piece(game_speed)
{
    type = L;
    for (int i=0;i<=2;i++)
    {
        Tile tile((5+i)*BLOC,BLOC,L);
        tile_list.push_back(tile);
    }
    Tile tile(5*BLOC,2*BLOC,L);
    tile_list.push_back(tile);
}

void L_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x+BLOC,y);
    tile_list[2].set(x+2*BLOC,y);
    tile_list[3].set(x,y+BLOC);
}


void L_piece::rotate_piece(vector<Tile> block_list)
{
    vector<Tile> tmp;
    tmp = tile_list;
    if (state == 0)
    {
        tile_list[0].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y - BLOC);
        tile_list[2].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y + BLOC);
        tile_list[3].set(tile_list[0].get_pos().x - BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 1;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 1)
    {
        tile_list[2].set(tile_list[1].get_pos().x - BLOC,tile_list[1].get_pos().y);
        tile_list[0].set(tile_list[1].get_pos().x + BLOC,tile_list[1].get_pos().y);
        tile_list[3].set(tile_list[0].get_pos().x ,tile_list[0].get_pos().y - BLOC);
        if (!test_rotation(block_list))
        {
            state = 2;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 2)
    {
        tile_list[2].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y - BLOC);
        tile_list[0].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y + BLOC);
        tile_list[3].set(tile_list[0].get_pos().x + BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 3;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 3)
    {
        tile_list[2].set(tile_list[1].get_pos().x + BLOC,tile_list[1].get_pos().y);
        tile_list[0].set(tile_list[1].get_pos().x - BLOC,tile_list[1].get_pos().y);
        tile_list[3].set(tile_list[0].get_pos().x,tile_list[0].get_pos().y + BLOC);
        if (!test_rotation(block_list))
        {
            state = 0;
            return;
        }
        tile_list = tmp;
    }
}

L_reverse_piece::L_reverse_piece(float game_speed):Piece(game_speed)
{
    type = LR;
    for (int i=2;i>=0;i--)
    {
        Tile tile((5+i)*BLOC,BLOC,LR);
        tile_list.push_back(tile);
    }
    Tile tile(7*BLOC,2*BLOC,LR);
    tile_list.push_back(tile);
}

void L_reverse_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x+BLOC,y);
    tile_list[2].set(x+2*BLOC,y);
    tile_list[3].set(x+2*BLOC,y+BLOC);
}

void L_reverse_piece::rotate_piece(vector<Tile> block_list)
{
    vector<Tile> tmp;
    tmp = tile_list;
    if (state == 0)
    {
        tile_list[0].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y - BLOC);
        tile_list[2].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y + BLOC);
        tile_list[3].set(tile_list[0].get_pos().x + BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 1;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 1)
    {
        tile_list[2].set(tile_list[1].get_pos().x  + BLOC,tile_list[1].get_pos().y);
        tile_list[0].set(tile_list[1].get_pos().x  - BLOC,tile_list[1].get_pos().y);
        tile_list[3].set(tile_list[0].get_pos().x,tile_list[0].get_pos().y - BLOC);
        if (!test_rotation(block_list))
        {
            state = 2;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 2)
    {
        tile_list[2].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y + BLOC);
        tile_list[0].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y - BLOC);
        tile_list[3].set(tile_list[2].get_pos().x - BLOC,tile_list[2].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 3;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 3)
    {
        tile_list[2].set(tile_list[1].get_pos().x - BLOC,tile_list[1].get_pos().y);
        tile_list[0].set(tile_list[1].get_pos().x + BLOC,tile_list[1].get_pos().y);
        tile_list[3].set(tile_list[0].get_pos().x,tile_list[0].get_pos().y + BLOC);
        if (!test_rotation(block_list))
        {
            state = 0;
            return;
        }
        tile_list = tmp;
    }
}

N_piece::N_piece(float game_speed):Piece(game_speed)
{
    type = N;
    Tile tile(5*BLOC,2*BLOC,NR);
    tile_list.push_back(tile);
    Tile tile1(5*BLOC,BLOC,NR);
    tile_list.push_back(tile1);
    Tile tile2(4*BLOC,BLOC,NR);
    tile_list.push_back(tile2);
    Tile tile3(6*BLOC,2*BLOC,NR);
    tile_list.push_back(tile3);
}

void N_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x,y-BLOC);
    tile_list[2].set(x-BLOC,y-BLOC);
    tile_list[3].set(x+BLOC,y);
}

void N_piece::rotate_piece(vector<Tile> block_list)
{
    vector<Tile> tmp;
    tmp = tile_list;
    if (state == 0)
    {
        tile_list[3].set(tile_list[2].get_pos().x,tile_list[2].get_pos().y + BLOC);
        tile_list[0].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y - BLOC);
        if (!test_rotation(block_list))
        {
            state = 1;
            return;
        }
        tile_list = tmp;

    }
    else if (state == 1)
    {
        tile_list[0].set(tile_list[1].get_pos().x,tile_list[1].get_pos().y + BLOC);
        tile_list[3].set(tile_list[0].get_pos().x + BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 0;
            return;
        }
        tile_list = tmp;
    }
}

N_reverse_piece::N_reverse_piece(float game_speed):Piece(game_speed)
{
    type = NR;
    Tile tile(5*BLOC,2*BLOC,N);
    tile_list.push_back(tile);
    Tile tile1(5*BLOC,BLOC,N);
    tile_list.push_back(tile1);
    Tile tile2(6*BLOC,BLOC,N);
    tile_list.push_back(tile2);
    Tile tile3(4*BLOC,2*BLOC,N);
    tile_list.push_back(tile3);
}

void N_reverse_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x,y-BLOC);
    tile_list[3].set(x-BLOC,y);
    tile_list[2].set(x+BLOC,y-BLOC);
}

void N_reverse_piece::rotate_piece(vector<Tile> block_list)
{
    vector<Tile> tmp;
    tmp = tile_list;
    if (state == 0)
    {
        tile_list[3].set(tile_list[2].get_pos().x,tile_list[2].get_pos().y + BLOC);
        tile_list[0].set(tile_list[1].get_pos().x ,tile_list[1].get_pos().y - BLOC);
        if (!test_rotation(block_list))
        {
            state = 1;
            return;
        }
        tile_list = tmp;

    }
    else if (state == 1)
    {
        tile_list[0].set(tile_list[1].get_pos().x,tile_list[1].get_pos().y + BLOC);
        tile_list[3].set(tile_list[0].get_pos().x - BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 0;
            return;
        }
        tile_list = tmp;
    }
}

T_piece::T_piece(float game_speed):Piece(game_speed)
{
    type = T;
    Tile tile(5*BLOC,2*BLOC,T);
    tile_list.push_back(tile);
    Tile tile1(5*BLOC,BLOC,T);
    tile_list.push_back(tile1);
    Tile tile2(6*BLOC,2*BLOC,T);
    tile_list.push_back(tile2);
    Tile tile3(4*BLOC,2*BLOC,T);
    tile_list.push_back(tile3);

}

void T_piece::set(float x,float y)
{
    tile_list[0].set(x,y);
    tile_list[1].set(x,y-BLOC);
    tile_list[2].set(x-BLOC,y);
    tile_list[3].set(x+BLOC,y);
}

void T_piece::rotate_piece(vector<Tile> block_list)
{
    vector<Tile> tmp;
    tmp = tile_list;
    if (state == 0)
    {
        tile_list[3].set(tile_list[0].get_pos().x ,tile_list[0].get_pos().y + BLOC);
        if (!test_rotation(block_list))
        {
            state = 1;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 1)
    {
        tile_list[1].set(tile_list[0].get_pos().x - BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 2;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 2)
    {
        tile_list[2].set(tile_list[0].get_pos().x ,tile_list[0].get_pos().y - BLOC);
        if (!test_rotation(block_list))
        {
            state = 3;
            return;
        }
        tile_list = tmp;
    }
    else if (state == 3)
    {
        tile_list[1].set(tile_list[0].get_pos().x,tile_list[0].get_pos().y - BLOC);
        tile_list[3].set(tile_list[0].get_pos().x - BLOC,tile_list[0].get_pos().y);
        tile_list[2].set(tile_list[0].get_pos().x + BLOC,tile_list[0].get_pos().y);
        if (!test_rotation(block_list))
        {
            state = 0;
            return;
        }
        tile_list = tmp;
    }
}
