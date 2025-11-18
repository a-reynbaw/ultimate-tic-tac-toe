#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include "state.h"

big_board bb;
int next_board_i = -1;
int next_board_j = -1;

// Helper function to call JS to update the status text
void update_status_js() {
    char js_code[256];
    if (bb->status == OVER) {
        // Final win/loss/draw message is handled by detect_bb
        return;
    }

    if (next_board_i == -1) {
        sprintf(js_code, "window.updateStatus('Player %c turn. Choose any valid board.');", (bb->currentPlayer == PLAYER_X) ? 'X' : 'O');
    } else {
        sprintf(js_code, "window.updateStatus('Player %c turn. Play in board (%d, %d).');", (bb->currentPlayer == PLAYER_X) ? 'X' : 'O', next_board_i, next_board_j);
    }
    emscripten_run_script(js_code);
}


// This function is called from JS when a user clicks a cell
EMSCRIPTEN_KEEPALIVE
void process_move(int i, int j, int x, int y) {
    if (bb->status == OVER) return;

    // --- Move Validation ---
    if (next_board_i != -1 && (i != next_board_i || j != next_board_j)) {
        emscripten_run_script("window.updateStatus('Invalid move: Must play in the highlighted board.');");
        return;
    }
    if (bb->smallBoards[i][j]->status != BOARD_START && bb->smallBoards[i][j]->status != IN_PROGRESS) {
        emscripten_run_script("window.updateStatus('Invalid move: This board is already finished.');");
        return;
    }
    if (bb->smallBoards[i][j]->board[x][y] != ' ') {
        emscripten_run_script("window.updateStatus('Invalid move: That cell is already taken.');");
        return;
    }

    // --- Play Move & Update UI ---
    play_move(bb, i, j, x, y);
    char js_code[128];
    sprintf(js_code, "window.updateCell(%d, %d, %d, %d, %d);", i, j, x, y, bb->currentPlayer);
    emscripten_run_script(js_code);

    bb->last_played->x = i;
    bb->last_played->y = j;
    bb->smallBoards[i][j]->last_played->x = x;
    bb->smallBoards[i][j]->last_played->y = y;
    if(bb->smallBoards[i][j]->status == BOARD_START) {
        bb->smallBoards[i][j]->status = IN_PROGRESS;
    }

    // --- Check for Small Board Win/Draw ---
    board_status sb_status = detect_sb(bb->smallBoards[i][j]);
    if (sb_status != IN_PROGRESS) {
        bb->smallBoards[i][j]->status = sb_status;
        sprintf(js_code, "window.markSmallBoardFinished(%d, %d, %d);", i, j, sb_status);
        emscripten_run_script(js_code);

        // --- Check for Overall Game Win/Draw ---
        board_status bb_status = detect_bb(bb);
        if (bb_status != IN_PROGRESS) {
            bb->status = OVER;
            if (bb_status == X_WON) sprintf(js_code, "window.updateStatus('GAME OVER: Player X Wins!');");
            else if (bb_status == O_WON) sprintf(js_code, "window.updateStatus('GAME OVER: Player O Wins!');");
            else if (bb_status == DRAW) sprintf(js_code, "window.updateStatus('GAME OVER: It is a Draw!');");
            emscripten_run_script(js_code);
        }
    }

    // --- Determine Next Board & Switch Player ---
    if (bb->status != OVER) {
        next_board_i = x;
        next_board_j = y;

        if (bb->smallBoards[next_board_i][next_board_j]->status != BOARD_START && bb->smallBoards[next_board_i][next_board_j]->status != IN_PROGRESS) {
            next_board_i = -1;
            next_board_j = -1;
        }
        
        // Highlight the next board on the UI
        sprintf(js_code, "window.highlightNextBoard(%d, %d);", next_board_i, next_board_j);
        emscripten_run_script(js_code);

        bb->currentPlayer = (bb->currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        update_status_js();
    }
}

// This function is called once from JS when the page loads
EMSCRIPTEN_KEEPALIVE
void initialize_game() {
    bb = create_big_board();
    bb->currentPlayer = PLAYER_X;
    next_board_i = -1;
    next_board_j = -1;

    // Set initial status message and highlight
    update_status_js();
    emscripten_run_script("window.highlightNextBoard(-1, -1);");
}

int main() {
    // All logic is initiated from JavaScript.
    return 0;
}