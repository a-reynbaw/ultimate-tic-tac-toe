#ifndef STATE_C
#define STATE_C

#include <stdio.h>
#include <stdlib.h>
#include "state.h"

/**
 * @brief creates a new small board
 * @return a pointer to the new small board
 * @note The small board is initialized with empty spaces and the current player is set to NONE. The status is set to IN_PROGRESS.
*/
small_board create_small_board() {
    small_board sb = (small_board)malloc(sizeof(struct sb));

    if (sb == NULL) {
        fprintf(stderr, "Memory allocation failed for small board\n");
        exit(EXIT_FAILURE);
    }

    // 3x3 board initaly filled with empty spaces
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            sb->board[i][j] = ' ';
        }
    }

    sb->last_played = (Vector2)malloc(sizeof(struct vector2));

    if (!sb->last_played) {
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    sb->last_played->x = -1;
    sb->last_played->y = -1;

    sb->status = BOARD_START;
    return sb;
}

/**
 * @brief creates a new big board
 * @return a pointer to the new big board
 * @note The big board is initialized with small boards and the status is set to PLAYING.
*/
big_board create_big_board() {
    big_board bb = (big_board)malloc(sizeof(struct bb));

    if (bb == NULL) {
        fprintf(stderr, "Memory allocation failed for big board\n");
        exit(EXIT_FAILURE);
    }

    // 3x3 small boards initaly filled with empty spaces
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            bb->smallBoards[i][j] = create_small_board();
        }
    }

    bb->last_played = (Vector2)malloc(sizeof(struct vector2));

    if (!bb->last_played) {
        fprintf(stderr, "Unable to allocate memory.\n");
        exit(EXIT_FAILURE);
    }

    bb->last_played->x = -1;
    bb->last_played->y = -1;

    bb->status = START;
    return bb;
}

/**
 * @brief prints the small board
 * @param sb the small board to print
 * @note The small board is printed in a 3x3 grid format.
*/
/*  NOT NEEDED  */
void print_small_board(small_board sb) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", sb->board[i][j]);
            if (j < 2) {
                printf("|");
            } else {
                printf("  ");
            }
        }
        printf("\n");
        if (i < 2) {
            printf("---|---|---  \n");
        } else {
            printf("  \n");
        }
    }
}

/**
 * @brief prints the big board
 * @param bb the big board to print
 * @note The big board is printed in a 3x3 grid format, with each small board printed in its place.
*/
void print_big_board(big_board bb) {
    for (int u = 0; u < 3; u++) {
        for (int k = 0; k < 3; k++) {
            for (int j = 0; j < 3; j++) {
                for (int i = 0; i < 3; i++) {
                    printf(" %c ", bb->smallBoards[u][j]->board[k][i]);

                    if (i < 2) {
                        printf("|");
                    } else {
                        printf("   ");
                    }
                
                }
            }

            printf("\n");
            if (k < 2) {
                printf("---|---|---   ---|---|---   ---|---|---   \n");
            } else {
                printf("  \n");
            }

        }
    }
    
}

/**
 * @brief destroys the small board
 * @param sb the small board to destroy
 * @note The small board is freed from memory.
*/
void destroy_small_board(small_board sb) {
    if (sb != NULL) {
        free(sb);
    }
}

/**
 * @brief destroys the big board
 * @param bb the big board to destroy
 * @note The big board is freed from memory.
*/
void destroy_big_board(big_board bb) {
    if (bb != NULL) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                destroy_small_board(bb->smallBoards[i][j]);
            }
        }
        free(bb);
    }
}

/**
 * @brief plays a move on the big board
 * @param bb the big board
 * @param i the x coordinate of the small board
 * @param j the y coordinate of the small board
 * @param x the x coordinate of the move
 * @param y the y coordinate of the move
 * @note The move is played on the small board at (x, y) and the current player is updated.
*/
void play_move(big_board bb, int i, int j, int x, int y) {
    // validate move
    if (i < 0 || i > 3 || j < 0 || j > 3 || x < 0 || x > 3 || y < 0 || y > 3) {
        fprintf(stderr, "Invalid coordinate(s).\n");
        return;
    }

    if (bb->smallBoards[i][j]->status != IN_PROGRESS && bb->smallBoards[i][j]->status != BOARD_START) {
        fprintf(stderr, "Small board is already finished.\n");
        return;
    }
    
    if (bb->smallBoards[i][j]->board[x][y] == ' ') {
        bb->smallBoards[i][j]->board[x][y] = (bb->currentPlayer == PLAYER_X) ? 'X' : 'O';
    } else {
        fprintf(stderr, "Already filled spot.\n");
    }

    if (bb->status == START) bb->status = PLAYING;
}

board_status detect_sb(small_board sb) {
    int x = sb->last_played->x, y = sb->last_played->y;

    if (x < 0 || x > 2 || y < 0 || y > 2) {
        fprintf(stderr, "Invalid coordinate(s).\n");
        return IN_PROGRESS;
    }

    // check rows
    if (sb->board[x][0] == sb->board[x][1] && sb->board[x][1] == sb->board[x][2]) {
        if (sb->board[x][0] == 'X') {
            return X_WON;
        } else if (sb->board[x][0] == 'O') {
            return O_WON;
        }
    }


    // check columns
    if (sb->board[0][y] == sb->board[1][y] && sb->board[1][y] == sb->board[2][y]) {
        if (sb->board[0][y] == 'X') {
            return X_WON;
        } else if (sb->board[0][y] == 'O') {
            return O_WON;
        }
    }


    // check diagonals
    if (x == y) {
        if (sb->board[0][0] == sb->board[1][1] && sb->board[1][1] == sb->board[2][2]) {
            if (sb->board[0][0] == 'X') {
                return X_WON;
            } else if (sb->board[0][0] == 'O') {
                return O_WON;
            }
        }
    }


    if (x + y == 2) {
        if (sb->board[0][2] == sb->board[1][1] && sb->board[1][1] == sb->board[2][0]) {
            if (sb->board[0][2] == 'X') {
                return X_WON;
            } else if (sb->board[0][2] == 'O') {
                return O_WON;
            }
        }
    }


    // check for draw
    int filled = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (sb->board[i][j] != ' ') {
                filled++;
            }
        }
    }


    if (filled == 9) {
        return DRAW;
    }

    return IN_PROGRESS;

}

/**
 * @brief detects the status of the big board
 * @param bb the big board
 * @note The status is updated based on the small boards.
*/
board_status detect_bb(big_board bb) {
    int x = bb->last_played->x, y = bb->last_played->y;

    if (x < 0 || x > 2 || y < 0 || y > 2) {
        fprintf(stderr, "Invalid coordinate(s).\n");
        return IN_PROGRESS;
    }

    // check rows
    if (bb->smallBoards[x][0]->status == bb->smallBoards[x][1]->status && bb->smallBoards[x][1]->status == bb->smallBoards[x][2]->status) {
        if (bb->smallBoards[x][0]->status == X_WON) {
            return X_WON;
        } else if (bb->smallBoards[x][0]->status == O_WON) {
            return O_WON;
        }
    }

    // check for colums
    if (bb->smallBoards[0][y]->status == bb->smallBoards[1][y]->status && bb->smallBoards[1][y]->status == bb->smallBoards[2][y]->status) {
        if (bb->smallBoards[0][y]->status == X_WON) {
            return X_WON;
        } else if (bb->smallBoards[0][y]->status == O_WON) {
            return O_WON;
        }
    }

    // check diagonals
    if (x == y) {
        if (bb->smallBoards[0][0]->status == bb->smallBoards[1][1]->status && bb->smallBoards[1][1]->status == bb->smallBoards[2][2]->status) {
            if (bb->smallBoards[0][0]->status == X_WON) {
                return X_WON;
            } else if (bb->smallBoards[0][0]->status == O_WON) {
                return O_WON;
            }
        }
    }

    if (x + y == 2) {
        if (bb->smallBoards[0][2]->status == bb->smallBoards[1][1]->status && bb->smallBoards[1][1]->status == bb->smallBoards[2][0]->status) {
            if (bb->smallBoards[0][2]->status == X_WON) {
                return X_WON;
            } else if (bb->smallBoards[0][2]->status == O_WON) {
                return O_WON;
            }
        }
    }

    // check for draw
    int filled = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (bb->smallBoards[i][j]->status == DRAW) {
                filled++;
            }
        }
    }
    if (filled == 9) {
        return DRAW;
    }
    return IN_PROGRESS;
}


#endif