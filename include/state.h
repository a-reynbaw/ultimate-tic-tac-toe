#ifndef STATE_H
#define STATE_H


typedef enum {
    START,
    PLAYING,
    PAUSED,
    OVER
} game_status;

typedef enum {
    BOARD_START,
    X_WON,
    O_WON,
    DRAW,
    IN_PROGRESS
} board_status;

typedef enum {
    PLAYER_X,
    PLAYER_O,
    NONE
} player;

typedef struct vector2 {
    int x;
    int y;
} * Vector2;

typedef struct sb {
    char board[3][3];

    Vector2 last_played;

    board_status status;
} * small_board;

typedef struct bb {
    small_board smallBoards[3][3];

    player currentPlayer;

    Vector2 last_played;

    game_status status;
} * big_board;

small_board create_small_board();
big_board create_big_board();

void print_small_board(small_board sb);
void print_big_board(big_board bb);
//void print_board(small_board sb, int x, int y);

void destroy_small_board(small_board sb);
void destroy_big_board(big_board bb);

void play_move(big_board, int, int, int, int);

board_status detect_sb(small_board);

board_status detect_bb(big_board);

#endif