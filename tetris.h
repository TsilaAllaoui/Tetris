#ifndef TETRIS_H
#define TETRIS_H

#include <ctime>
#include <SDL/SDL_ttf.h>
#include "defs.h"
#include "timer.h"
#include "tile.h"
#include "piece.h"

class Tetris
{
    private:
        SDL_Surface *screen, *sprite, *background, *score, *titlescreen;
;
        Timer timer;
        SDL_Rect pos_b;
        Piece *curr_piece, *next_piece, *stock_piece, *UI_piece, *UI_stock_piece;
        vector<Tile> block_list, tmp;
        float game_speed;
        int curr_score;
        TTF_Font *police;
    public:
        Tetris();
        void play();
        void update();
        void show();
        bool game_over();
        void erase_possible_line();
        void erase_element(SDL_Rect mpos);
        Piece* gen_next_Piece();
        bool is_at(float mx,float my);
        void show_Ui_piece();
        void show_stock_piece();
        void show_UI();
        int get_score_long();
        void set_tetris();
        bool end_tetris();
};

#endif // TETRIS_H
