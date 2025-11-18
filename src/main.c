#include <stdio.h>
#include <stdlib.h>
#include "state.h"


// set it as global
big_board bb;

void print_game() {
    while (1) {
        printf("\r");
        printf("                   _ _   _                 _                  \n");
        printf("             _   _| | |_(_)_ __ ___   __ _| |_ ___            \n");
        printf("            | | | | | __| | '_ ` _ \\ / _` | __/ _ \\           \n");
        printf("            | |_| | | |_| | | | | | | (_| | ||  __/           \n");
        printf("  _   _      \\__,_|_|\\__|_|_| |_| |_|\\__,_|\\__\\___|           \n");
        printf(" | |_(_) ___          | |_ __ _  ___          | |_ ___   ___  \n");
        printf(" | __| |/ __|  _____  | __/ _` |/ __|  _____  | __/ _ \\ / _ \\ \n");
        printf(" | |_| | (__  |_____| | || (_| | (__  |_____| | || (_) |  __/ \n");
        printf("  \\__|_|\\___|          \\__\\__,_|\\___|          \\__\\___/ \\___| \n");
        printf("\n");
        printf("---------------------------------------------------------------\n");
        printf("     0             1             2       \n");

        print_big_board(bb);


        printf("---------------------------------------------------------------\n");
        printf("Current Player: %s\n", (bb->currentPlayer == PLAYER_X) ? "X" : "O");

        int x, y, i, j;

        if (bb->status == START) {
            printf("Choose board to start playing: <x> <y>\n");
            
            scanf("%d %d", &i, &j);

            if (i < 0 || i > 2 || j < 0 || j > 2) {
                fprintf(stderr, "Invalid coordinate(s).\n");
                return;
            }

            printf("Insert your move: <x> <y> \n");

            scanf("%d %d", &x, &y);
                
            if (x < 0 || x > 2 || y < 0 || y > 2) {
                fprintf(stderr, "Invalid coordinate(s).\n");
                return;
            }

        } else {
            i = bb->smallBoards[bb->last_played->x][bb->last_played->y]->last_played->x;
            j = bb->smallBoards[bb->last_played->x][bb->last_played->y]->last_played->y;

            if (i == -1 && j == -1) {
                printf("Current Board: Whichever (in progress) board you want! \n");
                printf("Give board you want to play at: <x> <y> \n");
                scanf("%d %d", &i, &j);

                if (i < 0 || i > 2 || j < 0 || j > 2) {
                    fprintf(stderr, "Invalid coordinate(s).\n");
                    return;
                }

                if (bb->smallBoards[i][j]->status != IN_PROGRESS) {
                    fprintf(stderr, "Board is already finished.\n");
                    return;
                }

                printf("Insert your move: <x> <y> \n");

                scanf("%d %d", &x, &y);
                
                if (x < 0 || x > 2 || y < 0 || y > 2) {
                    fprintf(stderr, "Invalid coordinate(s).\n");
                    return;
                }

            } else {
                printf("Current Board: (%d, %d)\n", bb->smallBoards[bb->last_played->x][bb->last_played->y]->last_played->x, bb->smallBoards[bb->last_played->x][bb->last_played->y]->last_played->y);
                printf("Insert your move: <x> <y> \n");

                scanf("%d %d", &x, &y);
                
                if (x < 0 || x > 2 || y < 0 || y > 2) {
                    fprintf(stderr, "Invalid coordinate(s).\n");
                    return;
                }
            }

        }

        play_move(bb, i, j, x, y);

        bb->last_played->x = i;
        bb->last_played->y = j;

        bb->smallBoards[i][j]->last_played->x = x;
        bb->smallBoards[i][j]->last_played->y = y;

        board_status sb_status = detect_sb(bb->smallBoards[i][j]);


        if (sb_status != IN_PROGRESS) {
            //bb->smallBoards[i][j]->last_played->x = -1;
            //bb->smallBoards[i][j]->last_played->y = -1;

            bb->smallBoards[i][j]->status = sb_status;

            if (sb_status != DRAW) {

                board_status sb_status = detect_bb(bb);

                if (sb_status == X_WON) {
                    printf("PLAYER X WON!!!\n");
                    bb->status = OVER;
                    return;

                } else if (sb_status == O_WON) {
                    printf("PLAYER O WON!!!\n");
                    bb->status = OVER;
                    return;

                } else if (sb_status == DRAW) {
                    printf("DRAW!!! :(\n");
                    bb->status = OVER;
                    return;
                }
            }

        }

        bb->currentPlayer = !(bb->currentPlayer);
    }

}

int main() {
    bb = create_big_board();

    print_game();
    
    return 0;
}