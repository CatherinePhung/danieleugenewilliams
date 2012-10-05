/****************************************************************************
 * sudoku.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Implements the game of Sudoku.
 ***************************************************************************/

#include "sudoku.h"

#include <ctype.h>
#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// macro for processing control characters
#define CTRL(x) ((x) & ~0140)

// size of each int (in bytes) in *.bin files
#define INTSIZE 4


// wrapper for our game's globals
struct
{
    // the current level
    char *level;

    // the game's board
    int board[9][9];
    
    // copy of the game's board
    int board2[9][9];

    // the board's number
    int number;

    // the board's top-left coordinates
    int top, left;

    // the cursor's current location between (0,0) and (8,8)
    int y, x;
} g;


// prototypes
void change(int ch);
bool change_valid(int ch);
bool is_zero(void);
void copy_numbers(void);
void draw_grid(void);
void draw_borders(void);
void draw_logo(void);
void draw_numbers(void);
int get_board_column(int _y);
int get_board_row(int _x);
void hide_banner(void);
bool load_board(void);
void handle_signal(int signum);
void log_move(int ch);
void move_cursor(int key);
void redraw_all(void);
bool restart_game(void);
void show_banner(char *b);
void show_cursor(void);
void shutdown(void);
bool startup(void);
bool won(void);


/*
 * Main driver for the game.
 */

int
main(int argc, char *argv[])
{
    // define usage
    const char *usage = "Usage: sudoku n00b|l33t [#]\n";

    // ensure that number of arguments is as expected
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, usage);
        return 1;
    }

    // ensure that level is valid
    if (strcmp(argv[1], "debug") == 0)
        g.level = "debug";
    else if (strcmp(argv[1], "n00b") == 0)
        g.level = "n00b";
    else if (strcmp(argv[1], "l33t") == 0)
        g.level = "l33t";
    else
    {
        fprintf(stderr, usage);
        return 2;
    }

    // n00b and l33t levels have 1024 boards; debug level has 9
    int max = (strcmp(g.level, "debug") == 0) ? 9 : 1024;

    // ensure that #, if provided, is in [1, max]
    if (argc == 3)
    {
        // ensure n is integral
        char c;
        if (sscanf(argv[2], " %d %c", &g.number, &c) != 1)
        {
            fprintf(stderr, usage);
            return 3;
        }

        // ensure n is in [1, max]
        if (g.number < 1 || g.number > max)
        {
            fprintf(stderr, "That board # does not exist!\n");
            return 4;
        }

        // seed PRNG with # so that we get same sequence of boards
        srand(g.number);
    }
    else
    {
        // seed PRNG with current time so that we get any sequence of boards
        srand(time(NULL));

        // choose a random n in [1, max]
        g.number = rand() % max + 1;
    }

    // start up ncurses
    if (!startup())
    {
        fprintf(stderr, "Error starting up ncurses!\n");
        return 5;
    }

    // register handler for SIGWINCH (SIGnal WINdow CHanged)
    signal(SIGWINCH, (void (*)(int)) handle_signal);

    // start the first game.
    if (!restart_game())
    {
        shutdown();
        fprintf(stderr, "Could not load board from disk!\n");
        return 6;
    }
    redraw_all();

    // let the user play!
    int ch;
    do
    {
        // refresh the screen
        refresh();

        // get user's input
        ch = getch();

        // capitalize input to simplify cases
        ch = toupper(ch);

        // process user's input
        switch (ch)
        {
            // start a new game
            case 'N': 
                g.number = rand() % max + 1;
                if (!restart_game())
                {
                    shutdown();
                    fprintf(stderr, "Could not load board from disk!\n");
                    return 6;
                }
                break;

            // restart current game
            case 'R': 
                if (!restart_game())
                {
                    shutdown();
                    fprintf(stderr, "Could not load board from disk!\n");
                    return 6;
                }
                break;

            // let user manually redraw screen with ctrl-L
            case CTRL('l'):
                redraw_all();
                break;
                
            // KEY_DOWN
            case KEY_DOWN:                
            // KEY_UP
            case KEY_UP:
            // KEY_LEFT
            case KEY_LEFT:
            // KEY_RIGHT
            case KEY_RIGHT:
                move_cursor(ch);
                break;
            
            // 0-9
            case 48:
            case 49:
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
            case 56:
            case 57:
                change(ch);
                break;             
        }
        


        // log input (and board's state) if any was received this iteration
        if (ch != ERR)
            log_move(ch);
    }
    while (ch != 'Q');

    // shut down ncurses
    shutdown();

    // tidy up the screen (using ANSI escape sequences)
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    // that's all folks
    printf("\nkthxbai!\n\n");
    return 0;
}


/*
 * Change character. Do nothing if not allowed.
 */
 
void
change(int ch)
{
    
    // get x/y coordinates
    int _x, _y, _a, _b;
    
    // get x/y coordinates
    getyx(stdscr, _x, _y);
    
    // get character at cursor position
    //char c = inch();
    
    // if the current character is ./0 and the game has not been won
    if(is_zero() && !won())
    {
        // get row
        _a = get_board_row(_x);
        
        // get column
        _b = get_board_column(_y);
        
        // change '0' to '.'
        ch = (ch == 48) ? 46 : ch;
        
        // change KEY_BACKSPACE to '.'
        ch = (ch == KEY_BACKSPACE) ? 46 : ch;
        
        // change KEY_DC to '.'
        ch = (ch == KEY_DC) ? 46 : ch;
        
        // change character on sudoku board
        char num[2];
        sprintf(num, "%c", ch);
        g.board[_a][_b] = atoi(num);
        addch(ch);
        
        // get current coordinates
        getyx(stdscr, _x, _y);
        
        // back out of extra space
        _y--;
        
        // move back to added character
        move(_x, _y);
        
        // refresh window
        refresh();
    }   
}

/*
 * Checks the number, ch, inputed by user against row, 
 * column, and 3x3 box to see if it is valid.
 */

bool
change_valid(int n)
{

    char num[2];
    sprintf(num, "%c", n);
    
    int ch = atoi(num);

    // get x/y coordinates
    int _x, _y, _a, _b;
    
    // get x/y coordinates
    getyx(stdscr, _x, _y);
    
    // get row
    _a = get_board_row(_x);
    
    // get column
    _b = get_board_column(_y);
    
    // check row for validity
    for (int i = 0; i < 9; i++)
    {
        if((g.board[_a][i] == ch) && (i != _b))
            // user input a duplicate in the row
            return false;
    }
    
    // check column for validity
    for (int i = 0; i < 9; i++)
    {
        if((g.board[i][_b] == ch) && (i != _a))
            // user input a duplicate in the column
            return false;
    }
    
    // check 3x3 box for validity
    int _amin, _amax, _bmin, _bmax;
    
    // top 3x3 row
    if((_a >= 0) && (_a <= 2))
    {
        _amin = 0;
        _amax = 2; 
    }
    else if((_a >= 3) && (_a <= 5))
    {
        _amin = 3;
        _amax = 5; 
    }
    else if((_a >= 6) && (_a <= 8))
    {
        _amin = 6;
        _amax = 8; 
    }
        
    // left 3x3 box
    if((_b >= 0) && (_b <= 2))
    {
        _bmin = 0;
        _bmax = 2;
    }
    // middle 3x3 box
    else if ((_b >= 3) && (_b <=5))
    {
        _bmin = 3;
        _bmax = 5;
    }
    // right 3x3 box
    else if ((_b >= 6) && (_b <=8))
    {
        _bmin = 6;
        _bmax = 8;
    }



    for (int i = _amin; i <= _amax ; i++)
    {
        for (int j = _bmin; j <= _bmax; j++)
        {
            if((g.board[i][j] == ch) && (i != _b))
            // found a duplicate in the 3x3 box
            return false;
        }
    } 
    
    return true;
}

/*
 * Check if current character where cursor is located is ./0
 */
 
bool
is_zero(void)
{
    int _x, _y, _a, _b;
    
    // get x/y coordinates
    getyx(stdscr, _x, _y);
    
    // get row
    _a = get_board_row(_x);
    
    // get column
    _b = get_board_column(_y);
    
    // if current space is zero/blank, it can be edited
    if (g.board2[_a][_b] == 0)
        return true;
    else
        return false;
    
    return false;
}
 

/*
 * Copy g.board into g.board2
 */
 
void
copy_numbers(void)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            // copy g.board values to g.board2
            g.board2[i][j] = g.board[i][j];
        }
    }
}

/*
 * Return g.board coordinates by window coordinates for x
 */
 
int
get_board_row(int _x)
{
    int _a = 0;
    
    // get row
    switch(_x)
    {
        case 6:
            _a = 0;
            break;
            
        case 7:
            _a = 1;
            break;
            
        case 8:
            _a = 2;
            break;
            
        case 10:
            _a = 3;
            break;
            
        case 11:
            _a = 4;
            break;
            
        case 12:
            _a = 5;
            break;
            
        case 14:
            _a = 6;
            break;
        
        case 15:
            _a = 7;
            break;
            
        case 16:
            _a = 8;
            break; 
    }
    
    return _a;
}

/*
 * Return g.board coordinates by window coordinates for y
 */
 
int
get_board_column(int _y)
{
    int _b = 0;
    
    // get column
    switch(_y)
    {
        case 18:
            _b = 0;
            break;
        
        case 20:
            _b = 1;
            break;
            
        case 22:
            _b = 2;
            break;
            
        case 26:
            _b = 3;
            break;
            
        case 28:
            _b = 4;
            break;
            
        case 30:
            _b = 5;
            break;
            
        case 34:
            _b = 6;
            break;
        
        case 36:
            _b = 7;
            break;
            
        case 38:
            _b = 8;
            break;   
    }
    
    return _b;
}

/*
 * Determine if the game is won
 */
 
bool
won(void)
{
    // TODO: check whether the game has been won.
    // TODO: all of g.board are non-zero
    // TODO: display a congratulatory banner
    // TODO: prevent user from changing board further
    
    return false;
}

/*
 * Draw's the game's board.
 */

void
draw_grid(void)
{
    // get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // determine where top-left corner of board belongs 
    g.top = maxy/2 - 7;
    g.left = maxx/2 - 30;

    // enable color if possible
    if (has_colors())
        attron(COLOR_PAIR(PAIR_GRID));

    // print grid
    for (int i = 0 ; i < 3 ; ++i )
    {
        mvaddstr(g.top + 0 + 4 * i, g.left, "+-------+-------+-------+");
        mvaddstr(g.top + 1 + 4 * i, g.left, "|       |       |       |");
        mvaddstr(g.top + 2 + 4 * i, g.left, "|       |       |       |");
        mvaddstr(g.top + 3 + 4 * i, g.left, "|       |       |       |");
    }
    mvaddstr(g.top + 4 * 3, g.left, "+-------+-------+-------+" );

    // remind user of level and #
    char reminder[maxx+1];
    sprintf(reminder, "   playing %s #%d", g.level, g.number);
    mvaddstr(g.top + 14, g.left + 25 - strlen(reminder), reminder);

    // disable color if possible
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_GRID));
}


/*
 * Draws game's borders.
 */

void
draw_borders(void)
{
    // get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // enable color if possible (else b&w highlighting)
    if (has_colors())
    {
        attron(A_PROTECT);
        attron(COLOR_PAIR(PAIR_BORDER));
    }
    else
        attron(A_REVERSE);

    // draw borders
    for (int i = 0; i < maxx; i++)
    {
        mvaddch(0, i, ' ');
        mvaddch(maxy-1, i, ' ');
    }

    // draw header
    char header[maxx+1];
    sprintf(header, "%s by %s", TITLE, AUTHOR);
    mvaddstr(0, (maxx - strlen(header)) / 2, header);

    // draw footer
    mvaddstr(maxy-1, 1, "[N]ew Game   [R]estart Game");
    mvaddstr(maxy-1, maxx-13, "[Q]uit Game");

    // disable color if possible (else b&w highlighting)
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_BORDER));
    else
        attroff(A_REVERSE);
}


/*
 * Draws game's logo.  Must be called after draw_grid has been
 * called at least once.
 */

void
draw_logo(void)
{
    // determine top-left coordinates of logo
    int top = g.top + 2;
    int left = g.left + 30;

    // enable color if possible
    if (has_colors())
        attron(COLOR_PAIR(PAIR_LOGO));

    // draw logo
    mvaddstr(top + 0, left, "               _       _          ");
    mvaddstr(top + 1, left, "              | |     | |         ");
    mvaddstr(top + 2, left, " ___ _   _  __| | ___ | | ___   _ ");
    mvaddstr(top + 3, left, "/ __| | | |/ _` |/ _ \\| |/ / | | |");
    mvaddstr(top + 4, left, "\\__ \\ |_| | (_| | (_) |   <| |_| |");
    mvaddstr(top + 5, left, "|___/\\__,_|\\__,_|\\___/|_|\\_\\\\__,_|");

    // sign logo
    char signature[3+strlen(AUTHOR)+1];
    sprintf(signature, "by %s", AUTHOR);
    mvaddstr(top + 7, left + 35 - strlen(signature) - 1, signature);

    // disable color if possible
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_LOGO));
}


/*
 * Draw's game's numbers.  Must be called after draw_grid has been
 * called at least once.
 */

void
draw_numbers(void)
{
    // iterate over board's numbers
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            // determine char
            char c = (g.board[i][j] == 0) ? '.' : g.board[i][j] + '0';
            mvaddch(g.top + i + 1 + i/3, g.left + 2 + 2*(j + j/3), c);
            refresh();
        }
    }
}


/*
 * Designed to handles signals (e.g., SIGWINCH).
 */

void
handle_signal(int signum)
{
    // handle a change in the window (i.e., a resizing)
    if (signum == SIGWINCH)
        redraw_all();

    // re-register myself so this signal gets handled in future too
    signal(signum, (void (*)(int)) handle_signal);
}


/*
 * Hides banner.
 */

void
hide_banner(void)
{
    // get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // overwrite banner with spaces
    for (int i = 0; i < maxx; i++)
        mvaddch(g.top + 16, i, ' ');
}


/*
 * Loads current board from disk, returning true iff successful.
 */

bool
load_board(void)
{
    // open file with boards of specified level
    char filename[strlen(g.level) + 5];
    sprintf(filename, "%s.bin", g.level);
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return false;

    // determine file's size
    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);

    // ensure file is of expected size
    if (size % (81 * INTSIZE) != 0)
    {
        fclose(fp);
        return false;
    }

    // compute offset of specified board
    int offset = ((g.number - 1) * 81 * INTSIZE);

    // seek to specified board
    fseek(fp, offset, SEEK_SET);

    // read board into memory
    if (fread(g.board, 81 * INTSIZE, 1, fp) != 1)
    {
        fclose(fp);
        return false;
    }

    // w00t
    fclose(fp);
    return true;
}


/*
 * Logs input and board's state to log.txt to facilitate automated tests.
 */

void
log_move(int ch)
{
    // open log
    FILE *fp = fopen("log.txt", "a");
    if (fp == NULL)
        return;

    // log input
    fprintf(fp, "%d\n", ch);

    // log board
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
            fprintf(fp, "%d", g.board[i][j]);
        fprintf(fp, "\n");
    }

    // that's it
    fclose(fp);
}

/*
 * Moves cursor on the board.
 */
void
move_cursor(int key)
{

    // initialize variables
    int _x, _y, _newx, _newy;
    
    // get x/y coordinates
    getyx(stdscr, _x, _y);
    
    switch(key)
    {
        // KEY_DOWN
        case KEY_DOWN:
            _newx = _x + 1;
            
            if(_newx == 5)
                // skip top border line
                break;
            
            else if(_newx == 9)
                // skip 2nd border line
                _newx++;
            
            else if(_newx == 13)
                // skip 3rd border line
                _newx++;
                
            else if(_newx == 17)
                // skip bottom border line
                break;
                
            move(_newx, _y);
            break;
            
        // KEY_UP
        case KEY_UP:
            _newx = _x - 1;
            
            if(_newx == 5)
                // skip top border line
                break;
            
            else if(_newx == 9)
                // skip 2nd border line
                _newx--;
            
            else if(_newx == 13)
                // skip 3rd border line
                _newx--;
                
            else if(_newx == 17)
                // skip bottom border line
                break;
            move(_newx, _y);
            break;
        
        // KEY_LEFT
        case KEY_LEFT:
            _newy = _y - 2;
            
            if(_newy == 16)
                // skip left border line
                _newy -= 2;
                
            
            else if(_newy == 24)
                // skip 2nd border line
                _newy -= 2;
            
            else if(_newy == 32)
                // skip 3rd border line
                _newy -= 2;
                
            else if(_newy == 40)
                // skip right border line
                break;
                
            move(_x, _newy);
            break;
            
        // KEY_RIGHT
        case KEY_RIGHT:
            _newy = _y + 2;
            
            if(_newy == 16)
                // skip left border line
                break;
            
            else if(_newy == 24)
                // skip 2nd border line
                _newy += 2;
            
            else if(_newy == 32)
                // skip 3rd border line
                _newy += 2;
                
            else if(_newy == 40)
                // skip right border line
                break;
                
            move(_x, _newy);
            break;
    }
}


/*
 * (Re)draws everything on the screen.
 */

void
redraw_all(void)
{
    // reset ncurses
    endwin();
    refresh();

    // clear screen
    clear();

    // re-draw everything
    draw_borders();
    draw_grid();
    draw_logo();
    draw_numbers();
    copy_numbers();

    // show cursor
    show_cursor();
}


/*
 * (Re)starts current game, returning true iff succesful.
 */

bool
restart_game(void)
{
    // reload current game
    if (!load_board())
        return false;

    // redraw board
    draw_grid();
    draw_numbers();

    // get window's dimensions
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    // move cursor to board's center
    g.y = g.x = 4;
    show_cursor();

    // remove log, if any
    remove("log.txt");

    // w00t
    return true;
}


/*
 * Shows cursor at (g.y, g.x).
 */

void
show_cursor(void)
{
    // restore cursor's location
    move(g.top + g.y + 1 + g.y/3, g.left + 2 + 2*(g.x + g.x/3));
}


/*
 * Shows a banner.  Must be called after show_grid has been
 * called at least once.
 */

void
show_banner(char *b)
{
    // enable color if possible
    if (has_colors())
        attron(COLOR_PAIR(PAIR_BANNER));

    // determine where top-left corner of board belongs 
    mvaddstr(g.top + 16, g.left + 64 - strlen(b), b);

    // disable color if possible
    if (has_colors())
        attroff(COLOR_PAIR(PAIR_BANNER));
}


/*
 * Shuts down ncurses.
 */

void
shutdown(void)
{
    endwin();
}


/*
 * Starts up ncurses.  Returns true iff successful.
 */

bool
startup(void)
{
    // initialize ncurses
    if (initscr() == NULL)
        return false;

    // prepare for color if possible
    if (has_colors())
    {
        // enable color
        if (start_color() == ERR || attron(A_PROTECT) == ERR)
        {
            endwin();
            return false;
        }

        // initialize pairs of colors
        if (init_pair(PAIR_BANNER, FG_BANNER, BG_BANNER) == ERR ||
            init_pair(PAIR_GRID, FG_GRID, BG_GRID) == ERR ||
            init_pair(PAIR_BORDER, FG_BORDER, BG_BORDER) == ERR ||
            init_pair(PAIR_LOGO, FG_LOGO, BG_LOGO) == ERR)
        {
            endwin();
            return false;
        }
    }

    // don't echo keyboard input
    if (noecho() == ERR)
    {
        endwin();
        return false;
    }

    // disable line buffering and certain signals
    if (raw() == ERR)
    {
        endwin();
        return false;
    }

    // enable arrow keys
    if (keypad(stdscr, true) == ERR)
    {
        endwin();
        return false;
    }

    // wait 1000 ms at a time for input
    timeout(1000);

    // w00t
    return true;
}
