#include "tetris.h"
#include <SDL/SDL_ttf.h>

Tetris::Tetris()
{
    srand(time(0));
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_EnableKeyRepeat(100,100);
    screen = SDL_SetVideoMode(WIDHT,HEIGHT,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
    sprite = IMG_Load("./data/images/tile.png");
    background = IMG_Load("./data/images/bg.png");
    titlescreen = IMG_Load("./data/images/titlescreen.png");
    pos_b.x = S_WIDHT+BLOC;
    pos_b.y = 0;
    game_speed = 1000;
    curr_piece = gen_next_Piece();
    next_piece = gen_next_Piece();
    stock_piece = gen_next_Piece();
    curr_score = 0;
}

void Tetris::play()
{
    REDO: set_tetris();
    while(!game_over())
    {
        timer.start();
        update();
        show();
        if (1000/30 > timer.get_time())
            SDL_Delay(1000/30 - timer.get_time());
    }
    if (end_tetris())
        exit(EXIT_SUCCESS);
    goto REDO;
}

void Tetris::update()
{
    if (curr_piece->is_active())
    {
        if (curr_piece->get_exchange())
        {
            int mtype = curr_piece->get_type(), ntype = stock_piece->get_type();
            switch (ntype)
            {
                case SQUARE:
                    curr_piece = new Square_piece(game_speed);
                    break;
                case I:
                    curr_piece = new I_piece(game_speed);
                    break;
                case L:
                    curr_piece = new L_piece(game_speed);
                    break;
                case LR:
                    curr_piece = new L_reverse_piece(game_speed);
                    break;
                case N:
                    curr_piece = new N_piece(game_speed);
                    break;
                case NR:
                    curr_piece = new N_reverse_piece(game_speed);
                    break;
                case T:
                    curr_piece = new T_piece(game_speed);
                    break;
            }
            curr_piece->lock_swap();
            switch (mtype)
            {
                case SQUARE:
                    stock_piece = new Square_piece(game_speed);
                    break;
                case I:
                    stock_piece = new I_piece(game_speed);
                    break;
                case L:
                    stock_piece = new L_piece(game_speed);
                    break;
                case LR:
                    stock_piece = new L_reverse_piece(game_speed);
                    break;
                case N:
                    stock_piece = new N_piece(game_speed);
                    break;
                case NR:
                    stock_piece = new N_reverse_piece(game_speed);
                    break;
                case T:
                    stock_piece = new T_piece(game_speed);
                    break;
            }
        }
        curr_piece->update(block_list);
    }
    else
    {
        block_list.push_back(curr_piece->get_tile_list()[3]);
        block_list.push_back(curr_piece->get_tile_list()[2]);
        block_list.push_back(curr_piece->get_tile_list()[1]);
        block_list.push_back(curr_piece->get_tile_list()[0]);
        erase_possible_line();
        switch (next_piece->get_type())
            {
                case SQUARE:
                    curr_piece = new Square_piece(game_speed);
                    break;
                case I:
                    curr_piece = new I_piece(game_speed);
                    break;
                case L:
                    curr_piece = new L_piece(game_speed);
                    break;
                case LR:
                    curr_piece = new L_reverse_piece(game_speed);
                    break;
                case N:
                    curr_piece = new N_piece(game_speed);
                    break;
                case NR:
                    curr_piece = new N_reverse_piece(game_speed);
                    break;
                case T:
                    curr_piece = new T_piece(game_speed);
                    break;
            }
        next_piece = gen_next_Piece();
    }
}

void Tetris::show_UI()
{
    char score_actu[50] = "";
    SDL_BlitSurface(background,NULL,screen,NULL);
    police = TTF_OpenFont("./data/fonts/8bitsFont.ttf", 10);
    SDL_Color white = {108,108,144};
    sprintf(score_actu, "%d",curr_score);
    score = TTF_RenderText_Solid(police,score_actu,white);
    SDL_Rect pos_score;
    if (get_score_long() == 1)
        pos_score.x = 226;
    else if (get_score_long() == 2)
        pos_score.x = 222;
    else if (get_score_long() == 3)
        pos_score.x = 218;
    else if (get_score_long() == 4)
        pos_score.x = 214;
    else if (get_score_long() == 5)
        pos_score.x = 210;
    else if (get_score_long() == 6)
        pos_score.x = 208;
    pos_score.y = 224;
    SDL_BlitSurface(score,NULL,screen,&pos_score);
    show_Ui_piece();
    show_stock_piece();
}

void Tetris::show()
{
    show_UI();
    curr_piece->show(screen,sprite);
    for (int i=0; i<(int)block_list.size(); i++)
        block_list[i].show(screen,sprite);
    SDL_Flip(screen);
}

Piece* Tetris::gen_next_Piece()
{
    int random_choice = rand()%7;
    //random_choice = T;
    switch(random_choice)
    {
    case SQUARE:
        next_piece = new Square_piece(game_speed);
        UI_piece = new Square_piece(game_speed);
        break;
    case I:
        next_piece = new I_piece(game_speed);
        UI_piece = new I_piece(game_speed);
        break;
    case L:
        next_piece = new L_piece(game_speed);
        UI_piece = new L_piece(game_speed);
        break;
    case LR:
        next_piece = new L_reverse_piece(game_speed);
        UI_piece = new L_reverse_piece(game_speed);
        break;
    case N:
        next_piece = new N_piece(game_speed);
        UI_piece = new N_piece(game_speed);
        break;
    case NR:
        next_piece = new N_reverse_piece(game_speed);
        UI_piece = new N_reverse_piece(game_speed);
        break;
    case T:
        next_piece = new T_piece(game_speed);
        UI_piece = new T_piece(game_speed);
        break;
    }
    return next_piece;
}

bool Tetris::game_over()
{
    for (int i=0; i<(int)block_list.size(); i++)
        if (block_list[i].get_pos().y <= BLOC)
            return true;
    return false;
}

bool Tetris::is_at(float mx,float my)
{
    for (int i=0; i<(int)block_list.size(); i++)
        if (block_list[i].get_pos().x == mx && block_list[i].get_pos().y == my)
        {
            tmp.push_back(block_list[i]);
            return true;
        }
    return false;
}

void Tetris::erase_element(SDL_Rect mpos)
{
    for (int i=0;i<(int)block_list.size();i++)
        if (block_list[i].get_pos().x == mpos.x && block_list[i].get_pos().y == mpos.y)
        {
            block_list.erase(block_list.begin()+i);
            //TODO: add fancy animation
        }
}

void Tetris::erase_possible_line()
{
    int n_line = 0;
    for (int i=BLOC;i<HEIGHT/BLOC-1;i++)
        if (is_at(BLOC,i*BLOC) && is_at(2*BLOC,i*BLOC) && is_at(3*BLOC,i*BLOC) && is_at(4*BLOC,i*BLOC)
            && is_at(5*BLOC,i*BLOC) && is_at(6*BLOC,i*BLOC) && is_at(7*BLOC,i*BLOC)
            && is_at(8*BLOC,i*BLOC) && is_at(9*BLOC,i*BLOC) && is_at(10*BLOC,i*BLOC))
            {
                n_line ++;
                for (int j=0;j<=10;j++)
                {
                    SDL_Rect tmp;
                    tmp.x = j*BLOC;
                    tmp.y = i*BLOC;
                    erase_element(tmp);
                }
                for (int k=i-1;k>=0;k--)
                    for (int l=0;l<10;l++)
                        for (int m=0;m<(int)block_list.size();m++)
                            if (block_list[m].get_pos().y == k*BLOC)
                                block_list[m].set(block_list[m].get_pos().x, block_list[m].get_pos().y + BLOC);
            }
    if (n_line >= 4)
        curr_score += n_line*20;
    else if (n_line < 4)
        curr_score += n_line*10;
    tmp.clear();
}


void Tetris::show_Ui_piece()
{
    if (UI_piece->get_type() == SQUARE)
        UI_piece->set(217,41);
    else if (UI_piece->get_type() == I)
        UI_piece->set(225,26);
    else if (UI_piece->get_type() == L)
        UI_piece->set(209,42);
    else if (UI_piece->get_type() == LR)
        UI_piece->set(209,42);
    else if (UI_piece->get_type() == N)
        UI_piece->set(225,55);
    else if (UI_piece->get_type() == NR)
        UI_piece->set(225,55);
    else if (UI_piece->get_type() == T)
        UI_piece->set(225,55);
    UI_piece->show(screen,sprite);
}

void Tetris::show_stock_piece()
{
    if (stock_piece->get_type() == SQUARE)
        stock_piece->set(217,140);
    else if (stock_piece->get_type() == I)
        stock_piece->set(225,124);
    else if (stock_piece->get_type() == L)
        stock_piece->set(209,137);
    else if (stock_piece->get_type() == LR)
        stock_piece->set(209,137);
    else if (stock_piece->get_type() == N)
        stock_piece->set(225,152);
    else if (stock_piece->get_type() == NR)
        stock_piece->set(225,152);
    else if (stock_piece->get_type() == T)
        stock_piece->set(225,152);
    stock_piece->show(screen,sprite);
}

int Tetris::get_score_long()
{
    int count = 0, n, m = curr_score;
    do
    {
        n = m/10;
        m /= 10;
        count ++;
    }
    while(n != 0);
    return count;
}

void Tetris::set_tetris()
{
    SDL_Surface *msurface, *nsurface;
    char mtext[50] = "",ntext[50] ="";
    police = TTF_OpenFont("./data/fonts/font.ttf", 15);
    sprintf(mtext, "Press Start");
    sprintf(ntext, "Made By TsilaAllaoui");
    SDL_Rect mpos, npos;
    mpos.x = 75; npos.x = 25;
    mpos.y = 216; npos.y = 236;
    bool start = false;
    SDL_Event event;
    while (!start)
    {
        SDL_BlitSurface(titlescreen,NULL,screen,NULL);
        SDL_PollEvent(&event);
        if (event.type == SDL_KEYDOWN)
        {
            if (event.key.keysym.sym == SDLK_SPACE)
                start = true;
        }
        int random_red = rand()%256; 
        int random_green = rand()%256; 
        int random_blue = rand()%256; 
        SDL_Color mcolor = {(uint8_t)random_red, (uint8_t)random_green, (uint8_t)random_blue};
        msurface = TTF_RenderText_Solid(police,mtext,mcolor);
        nsurface = TTF_RenderText_Solid(police,ntext,mcolor);
        SDL_BlitSurface(msurface,NULL,screen,&mpos);
        SDL_BlitSurface(nsurface,NULL,screen,&npos);
        SDL_Flip(screen);
        SDL_Delay(500);
    }
    SDL_FreeSurface(msurface);
    SDL_FreeSurface(nsurface);
}

bool Tetris::end_tetris()
{
    SDL_Surface *end_screen;
    end_screen = IMG_Load("./data/end.png");
    SDL_BlitSurface(end_screen,NULL,screen,NULL);
    SDL_Flip(screen);
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_SPACE)
            return false;
        else if (event.key.keysym.sym == SDLK_ESCAPE)
            return true;
    }
}
