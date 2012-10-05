/***************************************************************************
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements The Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 ***************************************************************************/
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


// constants
#define DIM_MIN 3
#define DIM_MAX 9


// board
int board[DIM_MAX][DIM_MAX];

// winning board
int _winboard[DIM_MAX][DIM_MAX];

// dimensions
int d;

// blank coordinates
int _xblank, _yblank;


// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);


int
main(int argc, char *argv[])
{
    // greet user with instructions
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %d x %d and %d x %d, inclusive.\n",
         DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}


/*
 * Clears screen using ANSI escape sequences.
 */

void
clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}


/*
 * Greets player.
 */

void
greet(void)
{
    clear();
    printf("WELCOME TO THE GAME OF FIFTEEN\n");
    usleep(2000000);
}


/*
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */

void
init(void)
{
    // initialize board with values
    int tile = (d * d) - 1;
    int cnt = 1;

    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // assign values to array
            board[i][j] = tile;
            _winboard[i][j] = cnt;
            cnt++;
            
            // if this is the blank tile, assign underscore
            if(tile == 0) 
            {
                board[i][j] = '_';
                _winboard[i][j] = '_';
                _xblank = i;
                _yblank = j; 
            }
            
            // decrement the tile values
            tile--;
            // printf("board[%d][%d]: %d\n", i, j, board[i][j]);
        }
    }
    
    // reset tile value
    tile = (d * d) - 1;
    
    // if tiles are odd, swap tiles 1 and 2
    if(tile % 2 == 1)
    {
        board[d-1][d-3] = 1;
        board[d-1][d-2] = 2;
        
        // printf("mod: %d\n", (tile % 2));
        // printf("board[%d][%d]: %d\n", d-1, d-3, board[d-1][d-3]);
        // printf("board[%d][%d]: %d\n", d-1, d-2, board[d-1][d-2]);
    }

}

/* 
 * Prints the board in its current state.
 */

void
draw(void)
{
    // print the board to the screen
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if(board[i][j] == 95)
                printf("%4c", '_');
            else
                printf("%4d", board[i][j]);
        }
        printf("\n");
    }
}


/* 
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */

bool
move(int tile)
{
    
    
    // get position of tile
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (tile == board[i][j])
            {
                //printf("i :%d, _xblank: %d, _yblank: %d, j: %d\n", i, _xblank, _yblank, j);
                // if on same row or column, directly adjacent
                if (((i == _xblank) && (abs(_yblank - j == 1))) || 
                    ((j == _yblank) && (abs(_xblank - i == 1))) ||
                    ((i == _xblank) && (abs(j - _yblank == 1))) ||
                    ((j == _yblank) && (abs(i - _xblank == 1))))
                {
                    // swap with blank tile
                    board[_xblank][_yblank] = tile;
                    board[i][j] = '_';
                    
                    // update blank coordinates
                    _xblank = i;
                    _yblank = j;
                    
                    return true;
                }
                               
            }
        }
    }

    // user entered an invalid move
    return false;
}


/*
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */

bool
won(void)
{
    // tile tracker / counter
    int won_cnt = 0;
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            // printf("%d - %d = %d\n", board[i][j], _winboard[i][j], board[i][j] - _winboard[i][j]);
            // for each tile in the correct place, increment won_cnt
            if(board[i][j] - _winboard[i][j] == 0)
                won_cnt++;
        }
        // printf("won_cnt: %d\n", won_cnt);
        // when won_cnt == (d*d), return true: game won!
        if(won_cnt == (d * d))
            return true;

    }
    
    return false;
    
}
